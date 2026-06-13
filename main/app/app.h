/**
 * @file app.h
 * @brief Application layer header
 */

#pragma once

#include "app_events.h"
#include "esp_err.h"

/**
 * @brief Initialize application subsystems
 */
esp_err_t app_init(void);

/**
 * @brief Application tick (called periodically)
 */
void app_tick(void);

/**
 * @brief Get application uptime in seconds
 */
uint32_t app_get_uptime(void);
