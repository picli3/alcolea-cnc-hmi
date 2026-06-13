/**
 * @file state_running.c
 * @brief RUNNING state implementation
 */

#include "cnc_state_machine.h"
#include "esp_log.h"

static const char* TAG = "state_running";

/**
 * @brief RUNNING state entry
 */
esp_err_t state_running_enter(void)
{
    ESP_LOGI(TAG, "RUNNING state entry");
    return ESP_OK;
}

/**
 * @brief RUNNING state exit
 */
esp_err_t state_running_exit(void)
{
    ESP_LOGI(TAG, "RUNNING state exit");
    return ESP_OK;
}

/**
 * @brief RUNNING state loop
 */
void state_running_loop(void)
{
    /* Stream G-code and update progress */
}

/**
 * @brief RUNNING state handle event
 */
esp_err_t state_running_on_event(cnc_event_t event, const cnc_event_data_t* data)
{
    switch (event) {
        case EVT_GCODE_STREAM_PAUSE:
            /* Transition to PAUSED */
            break;
        case EVT_GCODE_STREAM_COMPLETE:
            /* Transition to IDLE */
            break;
        case EVT_GCODE_STREAM_ABORT:
            /* Transition to IDLE */
            break;
        case EVT_GRBL_STATUS_UPDATE:
            /* Check for alarm */
            break;
        default:
            break;
    }
    return ESP_OK;
}
