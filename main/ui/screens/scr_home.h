/**
 * @file scr_home.h
 * @brief Home screen header
 */

#pragma once

#include "lvgl/lvgl.h"

/**
 * @brief Create home screen
 */
void scr_home_create(void);

/**
 * @brief Get home screen object
 */
lv_obj_t* scr_home_get_screen(void);

/**
 * @brief Update status on home screen
 */
void scr_home_update_status(const char* status, uint16_t buf_usage);

/**
 * @brief Update position display
 */
void scr_home_update_position(float x, float y, float z);

/**
 * @brief Update connection indicator
 */
void scr_home_update_connection(bool connected);
