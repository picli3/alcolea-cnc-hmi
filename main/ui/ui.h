/**
 * @file ui.h
 * @brief LVGL UI Layer header
 */

#pragma once

#include "esp_err.h"
#include "lvgl/lvgl.h"

/**
 * @brief Initialize UI
 */
esp_err_t ui_init(void);

/**
 * @brief UI tick (call in main loop)
 */
void ui_tick(void);

/**
 * @brief Update status display
 */
void ui_update_status(const char* status_str, uint16_t planner_buf, uint16_t rx_buf);

/**
 * @brief Update position display
 */
void ui_update_position(float x, float y, float z, bool is_work);

/**
 * @brief Update connection state
 */
void ui_update_connection(bool connected);

/**
 * @brief Get screen width
 */
int ui_get_width(void);

/**
 * @brief Get screen height
 */
int ui_get_height(void);
