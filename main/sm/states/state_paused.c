/**
 * @file state_paused.c
 * @brief PAUSED state implementation
 */

#include "cnc_state_machine.h"
#include "esp_log.h"

static const char* TAG = "state_paused";

/**
 * @brief PAUSED state entry
 */
esp_err_t state_paused_enter(void)
{
    ESP_LOGI(TAG, "PAUSED state entry");
    return ESP_OK;
}

/**
 * @brief PAUSED state exit
 */
esp_err_t state_paused_exit(void)
{
    ESP_LOGI(TAG, "PAUSED state exit");
    return ESP_OK;
}

/**
 * @brief PAUSED state loop
 */
void state_paused_loop(void)
{
    /* Show progress, retain position */
}

/**
 * @brief PAUSED state handle event
 */
esp_err_t state_paused_on_event(cnc_event_t event, const cnc_event_data_t* data)
{
    switch (event) {
        case EVT_GCODE_STREAM_RESUME:
            /* Transition to RUNNING */
            break;
        case EVT_GCODE_STREAM_ABORT:
            /* Transition to IDLE */
            break;
        default:
            break;
    }
    return ESP_OK;
}
