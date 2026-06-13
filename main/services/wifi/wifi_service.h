/**
 * @file wifi_service.h
 * @brief WiFi service header (Phase 3)
 *
 * Manages ESP32-C6 coprocessor via SDIO ESP-Hosted
 */

#pragma once

#include "esp_err.h"

/**
 * @brief File received callback
 */
typedef void (*file_received_cb_t)(const char* filename, void* user_data);

/**
 * @brief Initialize WiFi service
 */
esp_err_t wifi_service_init(void);

/**
 * @brief Start as Access Point
 */
esp_err_t wifi_start_ap(const char* ssid, const char* pass);

/**
 * @brief Start as Station
 */
esp_err_t wifi_start_sta(const char* ssid, const char* pass);

/**
 * @brief Get connected STA count
 */
int wifi_get_sta_count(void);

/**
 * @brief Register file received callback
 */
esp_err_t wifi_register_file_received_cb(file_received_cb_t cb, void* user_data);
