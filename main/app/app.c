/**
 * @file app.c
 * @brief Application layer implementation
 */

#include "app.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

static const char* TAG = "app";

static uint32_t s_app_start_time;
static QueueHandle_t s_event_queue;
static cnc_event_cb_t s_event_callbacks[4];
static void* s_event_user_data[4];
static uint8_t s_event_cb_count;

esp_err_t app_init(void)
{
    ESP_LOGI(TAG, "Initializing application");

    s_app_start_time = esp_timer_get_time() / 1000000;

    /* Create event queue */
    s_event_queue = xQueueCreate(16, sizeof(cnc_event_data_t));
    if (s_event_queue == NULL) {
        ESP_LOGE(TAG, "Failed to create event queue");
        return ESP_ERR_NO_MEM;
    }

    /* Initialize subsystems */
    ESP_LOGI(TAG, "Application initialized successfully");

    return ESP_OK;
}

void app_tick(void)
{
    /* Process events */
    cnc_event_data_t event;
    while (xQueueReceive(s_event_queue, &event, 0) == pdTRUE) {
        for (int i = 0; i < s_event_cb_count; i++) {
            if (s_event_callbacks[i]) {
                s_event_callbacks[i](event.type, &event, s_event_user_data[i]);
            }
        }
    }
}

uint32_t app_get_uptime(void)
{
    return (esp_timer_get_time() / 1000000) - s_app_start_time;
}

esp_err_t app_events_register(cnc_event_cb_t callback, void* user_data)
{
    if (s_event_cb_count >= 4) {
        return ESP_ERR_NO_MEM;
    }
    s_event_callbacks[s_event_cb_count] = callback;
    s_event_user_data[s_event_cb_count] = user_data;
    s_event_cb_count++;
    return ESP_OK;
}

esp_err_t app_events_unregister(cnc_event_cb_t callback)
{
    for (int i = 0; i < s_event_cb_count; i++) {
        if (s_event_callbacks[i] == callback) {
            for (int j = i; j < s_event_cb_count - 1; j++) {
                s_event_callbacks[j] = s_event_callbacks[j + 1];
                s_event_user_data[j] = s_event_user_data[j + 1];
            }
            s_event_cb_count--;
            return ESP_OK;
        }
    }
    return ESP_ERR_NOT_FOUND;
}

esp_err_t app_events_post(cnc_event_t event, const void* data, size_t len)
{
    cnc_event_data_t event_data = {
        .type = event,
        .data = (void*)data,
        .len = len
    };
    if (xQueueSend(s_event_queue, &event_data, 0) != pdTRUE) {
        return ESP_ERR_TIMEOUT;
    }
    return ESP_OK;
}
