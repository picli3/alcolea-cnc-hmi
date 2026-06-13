/**
 * @file state_connected.c
 * @brief CONNECTED state implementation
 */

#include "cnc_state_machine.h"
#include "esp_log.h"

static const char* TAG = "state_connected";

/**
 * @brief CONNECTED state entry
 */
esp_err_t state_connected_enter(void)
{
    ESP_LOGI(TAG, "CONNECTED state entry");
    return ESP_OK;
}

/**
 * @brief CONNECTED state exit
 */
esp_err_t state_connected_exit(void)
{
    ESP_LOGI(TAG, "CONNECTED state exit");
    return ESP_OK;
}

/**
 * @brief CONNECTED state loop
 */
void state_connected_loop(void)
{
    /* Monitor GRBL connection and file loading */
}

/**
 * @brief CONNECTED state handle event
 */
esp_err_t state_connected_on_event(cnc_event_t event, const cnc_event_data_t* data)
{
    switch (event) {
        case EVT_GRBL_DISCONNECTED:
            /* Transition to IDLE */
            break;
        case EVT_GCODE_STREAM_START:
            /* Transition to RUNNING */
            break;
        default:
            break;
    }
    return ESP_OK;
}
