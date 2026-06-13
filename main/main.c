/**
 * @file main.c
 * @brief CNC HMI Application Entry Point
 * @version 1.0
 * @date 2026-06-13
 */

#include "main.h"
#include "app_events.h"

static const char* TAG = "main";

/* System event group */
EventGroupHandle_t s_system_event_group = NULL;

/**
 * @brief Application entry point
 */
void app_main(void)
{
    esp_err_t ret;

    ESP_LOGI(TAG, "===========================================");
    ESP_LOGI(TAG, " %s v%s", APP_NAME, APP_VERSION);
    ESP_LOGI(TAG, " ESP32-P4 + LVGL 9.x + GRBL 1.1h");
    ESP_LOGI(TAG, "===========================================");

    /* Create system event group */
    s_system_event_group = xEventGroupCreate();
    assert(s_system_event_group != NULL);

    /* Initialize NVS */
    ret = nvs_flash_init();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "NVS initialization failed: %s", esp_err_to_name(ret));
    }

    /* Initialize application */
    ret = app_init();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Application initialization failed: %s", esp_err_to_name(ret));
        return;
    }

    /* Create application tasks */
    xTaskCreatePinnedToCore(
        app_main_loop,
        "app_main_loop",
        4096,
        NULL,
        3,
        NULL,
        0
    );

    ESP_LOGI(TAG, "Application started successfully");
}

/**
 * @brief Application main loop task
 */
void app_main_loop(void* pvParameters)
{
    ESP_LOGI(TAG, "Main loop task started");

    /* Start state machine */
    cnc_state_machine_init();

    /* Start GRBL manager */
    grbl_manager_init();

    /* Start UI */
    ui_init();

    /* Set boot complete bit */
    xEventGroupSetBits(s_system_event_group, SYSTEM_BOOT_BIT);

    /* Main loop - monitor system health */
    uint32_t loop_count = 0;
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
        loop_count++;

        if (loop_count % 10 == 0) {
            ESP_LOGD(TAG, "System heartbeat - uptime: %lu seconds", loop_count);
        }
    }
}
