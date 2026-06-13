/**
 * @file wgt_progress.h
 * @brief Progress widget header
 */

#pragma once

#include "lvgl/lvgl.h"

/**
 * @brief Create progress widget
 */
lv_obj_t* wgt_progress_create(lv_obj_t* parent);

/**
 * @brief Update progress (0.0 - 1.0)
 */
void wgt_progress_update(lv_obj_t* widget, float progress);
