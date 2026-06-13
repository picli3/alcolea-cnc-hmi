/**
 * @file state_error.c
 * @brief ERROR state implementation
 */

#include "cnc_state_machine.h"
#include "esp_log.h"

static const char* TAG = "state_error";

/**
 * @brief ERROR state entry
 */
esp_err_t state_error_enter(void)
{
    ESP_LOGE(TAG, "ERROR state entry");
    return ESP_OK;
}

/**
 * @brief ERROR state exit
 */
esp_err_t state_error_exit(void)
{
    ESP_LOGI(TAG, "ERROR state exit");
    return ESP_OK;
}

/**
 * @brief ERROR state loop
 */
void state_error_loop(void)
{
    /* Log error, show message */
}

/**
 * @brief ERROR state handle event
 */
esp_err_t state_error_on_event(cnc_event_t event, const cnc_event_data_t* data)
{
    switch (event) {
        case EVT_GRBL_DISCONNECTED:
            /* Transition to IDLE */
            break;
        default:
            break;
    }
    return ESP_OK;
}
