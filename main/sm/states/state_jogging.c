/**
 * @file state_jogging.c
 * @brief JOGGING state implementation
 */

#include "cnc_state_machine.h"
#include "esp_log.h"

static const char* TAG = "state_jogging";

/**
 * @brief JOGGING state entry
 */
esp_err_t state_jogging_enter(void)
{
    ESP_LOGI(TAG, "JOGGING state entry");
    return ESP_OK;
}

/**
 * @brief JOGGING state exit
 */
esp_err_t state_jogging_exit(void)
{
    ESP_LOGI(TAG, "JOGGING state exit");
    return ESP_OK;
}

/**
 * @brief JOGGING state loop
 */
void state_jogging_loop(void)
{
    /* Process jog commands */
}

/**
 * @brief JOGGING state handle event
 */
esp_err_t state_jogging_on_event(cnc_event_t event, const cnc_event_data_t* data)
{
    switch (event) {
        case EVT_JOG_STOP:
            /* Transition back to IDLE */
            break;
        default:
            break;
    }
    return ESP_OK;
}
