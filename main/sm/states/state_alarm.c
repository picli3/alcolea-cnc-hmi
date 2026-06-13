/**
 * @file state_alarm.c
 * @brief ALARM state implementation
 */

#include "cnc_state_machine.h"
#include "esp_log.h"

static const char* TAG = "state_alarm";

/**
 * @brief ALARM state entry
 */
esp_err_t state_alarm_enter(void)
{
    ESP_LOGW(TAG, "ALARM state entry");
    return ESP_OK;
}

/**
 * @brief ALARM state exit
 */
esp_err_t state_alarm_exit(void)
{
    ESP_LOGI(TAG, "ALARM state exit");
    return ESP_OK;
}

/**
 * @brief ALARM state loop
 */
void state_alarm_loop(void)
{
    /* Show alarm message, beep */
}

/**
 * @brief ALARM state handle event
 */
esp_err_t state_alarm_on_event(cnc_event_t event, const cnc_event_data_t* data)
{
    switch (event) {
        case EVT_ALARM_CLEARED:
            /* Send $X, transition to IDLE */
            break;
        case EVT_GRBL_DISCONNECTED:
            /* Transition to IDLE */
            break;
        default:
            break;
    }
    return ESP_OK;
}
