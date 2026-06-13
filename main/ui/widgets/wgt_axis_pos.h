/**
 * @file wgt_axis_pos.h
 * @brief Axis position widget header
 */

#pragma once

#include "lvgl/lvgl.h"

/**
 * @brief Create axis position widget
 */
lv_obj_t* wgt_axis_pos_create(lv_obj_t* parent);

/**
 * @brief Update axis position
 */
void wgt_axis_pos_update(lv_obj_t* widget, float x, float y, float z, bool is_work);
