/**
 * @file transitions.c
 * @brief State machine transitions
 */

#include "cnc_state_machine.h"
#include "esp_log.h"

static const char* TAG = "transitions";

/**
 * @brief Transition table validation
 *
 * Current State    | Event               | Action        | Next State
 * -----------------|---------------------|---------------|----------------
 * BOOT             | BOOT_COMPLETE       | Init GRBL     | IDLE
 * IDLE             | GRBL_CONNECTED       | -             | CONNECTED
 * IDLE             | GCODE_FILE_LOADED   | Load preview  | IDLE
 * IDLE             | JOG_START           | Enable jog    | IDLE
 * IDLE             | HOME_CYCLE          | Send $H       | IDLE
 * IDLE             | ZERO_SET            | Send G10 L20  | IDLE
 * IDLE             | STREAM_START        | Start stream  | RUNNING
 * CONNECTED        | GRBL_DISCONNECTED   | -             | IDLE
 * CONNECTED        | STREAM_START        | Start stream  | RUNNING
 * RUNNING          | STREAM_PAUSE        | Send !        | PAUSED
 * RUNNING          | STREAM_COMPLETE     | Log complete  | IDLE
 * RUNNING          | STREAM_ABORT        | Send 0x85     | IDLE
 * RUNNING          | STATUS_UPDATE(Alarm)| -             | ALARM
 * PAUSED           | STREAM_RESUME        | Send ~        | RUNNING
 * PAUSED           | STREAM_ABORT        | Send 0x85     | IDLE
 * PAUSED           | STATUS_UPDATE(Alarm)| -             | ALARM
 * ALARM            | ALARM_CLEARED       | Send $X       | IDLE
 * ALARM            | GRBL_DISCONNECTED   | -             | IDLE
 * *                 | ERROR_OCCURRED      | Log, notify   | ERROR
 * ERROR            | GRBL_DISCONNECTED   | -             | IDLE
 */

/**
 * @brief Execute transition
 */
esp_err_t transitions_execute(cnc_state_t current_state, cnc_event_t event,
                              cnc_state_t* next_state)
{
    esp_err_t ret = ESP_OK;

    /* Default: stay in current state */
    *next_state = current_state;

    switch (current_state) {
        case CNC_STATE_BOOT:
            if (event == EVT_SYSTEM_BOOT_COMPLETE) {
                *next_state = CNC_STATE_IDLE;
            }
            break;

        case CNC_STATE_IDLE:
            if (event == EVT_GRBL_CONNECTED) {
                *next_state = CNC_STATE_CONNECTED;
            } else if (event == EVT_GCODE_STREAM_START) {
                *next_state = CNC_STATE_RUNNING;
            }
            break;

        case CNC_STATE_CONNECTED:
            if (event == EVT_GRBL_DISCONNECTED) {
                *next_state = CNC_STATE_IDLE;
            } else if (event == EVT_GCODE_STREAM_START) {
                *next_state = CNC_STATE_RUNNING;
            }
            break;

        case CNC_STATE_RUNNING:
            if (event == EVT_GCODE_STREAM_PAUSE) {
                *next_state = CNC_STATE_PAUSED;
            } else if (event == EVT_GCODE_STREAM_COMPLETE ||
                       event == EVT_GCODE_STREAM_ABORT) {
                *next_state = CNC_STATE_IDLE;
            } else if (event == EVT_GRBL_STATUS_UPDATE) {
                /* Check for alarm in data */
                *next_state = CNC_STATE_ALARM;
            }
            break;

        case CNC_STATE_PAUSED:
            if (event == EVT_GCODE_STREAM_RESUME) {
                *next_state = CNC_STATE_RUNNING;
            } else if (event == EVT_GCODE_STREAM_ABORT) {
                *next_state = CNC_STATE_IDLE;
            }
            break;

        case CNC_STATE_ALARM:
            if (event == EVT_ALARM_CLEARED || event == EVT_GRBL_DISCONNECTED) {
                *next_state = CNC_STATE_IDLE;
            }
            break;

        case CNC_STATE_ERROR:
            if (event == EVT_GRBL_DISCONNECTED) {
                *next_state = CNC_STATE_IDLE;
            }
            break;

        default:
            break;
    }

    if (*next_state != current_state) {
        ESP_LOGI(TAG, "Transition: %s + %s -> %s",
                 cnc_state_get_name(current_state),
                 "event",
                 cnc_state_get_name(*next_state));
    }

    return ret;
}
