/**
 * @file wifi_service.c
 * @brief WiFi service implementation (Phase 3)
 */

#include "wifi_service.h"
#include "esp_log.h"

static const char* TAG = "wifi_service";

esp_err_t wifi_service_init(void)
{
    ESP_LOGI(TAG, "WiFi service placeholder - Phase 3");
    return ESP_OK;
}

esp_err_t wifi_start_ap(const char* ssid, const char* pass)
{
    ESP_LOGI(TAG, "WiFi AP placeholder: %s", ssid);
    return ESP_OK;
}

esp_err_t wifi_start_sta(const char* ssid, const char* pass)
{
    ESP_LOGI(TAG, "WiFi STA placeholder: %s", ssid);
    return ESP_OK;
}

int wifi_get_sta_count(void)
{
    return 0;
}

esp_err_t wifi_register_file_received_cb(file_received_cb_t cb, void* user_data)
{
    return ESP_OK;
}
