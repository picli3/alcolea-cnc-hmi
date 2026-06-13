/**
 * @file state_idle.c
 * @brief IDLE state implementation
 */

#include "cnc_state_machine.h"
#include "esp_log.h"

static const char* TAG = "state_idle";

/**
 * @brief IDLE state entry
 */
esp_err_t state_idle_enter(void)
{
    ESP_LOGI(TAG, "IDLE state entry");
    return ESP_OK;
}

/**
 * @brief IDLE state exit
 */
esp_err_t state_idle_exit(void)
{
    ESP_LOGI(TAG, "IDLE state exit");
    return ESP_OK;
}

/**
 * @brief IDLE state loop
 */
void state_idle_loop(void)
{
    /* Monitor GRBL connection */
}

/**
 * @brief IDLE state handle event
 */
esp_err_t state_idle_on_event(cnc_event_t event, const cnc_event_data_t* data)
{
    switch (event) {
        case EVT_GRBL_CONNECTED:
            /* Transition to CONNECTED */
            break;
        case EVT_GCODE_FILE_LOADED:
            /* Load preview */
            break;
        case EVT_JOG_START:
            /* Enable jog controls */
            break;
        case EVT_HOME_CYCLE:
            /* Send $H */
            break;
        case EVT_ZERO_SET:
            /* Send G10 L20 */
            break;
        case EVT_GCODE_STREAM_START:
            /* Transition to RUNNING */
            break;
        default:
            break;
    }
    return ESP_OK;
}
