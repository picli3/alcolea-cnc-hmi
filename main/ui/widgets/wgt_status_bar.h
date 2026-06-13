/**
 * @file wgt_status_bar.h
 * @brief Status bar widget header
 */

#pragma once

#include "lvgl/lvgl.h"

/**
 * @brief Create status bar widget
 */
lv_obj_t* wgt_status_bar_create(lv_obj_t* parent);

/**
 * @brief Update status bar
 */
void wgt_status_bar_update(lv_obj_t* widget, const char* grbl_status,
                           uint16_t planner_buf, uint16_t rx_buf);
