/**
 * @file wgt_jog_pad.h
 * @brief Jog pad widget header
 */

#pragma once

#include "lvgl/lvgl.h"

/**
 * @brief Jog pad callback
 */
typedef void (*jog_callback_t)(int axis, float step_mm, void* user_data);

/**
 * @brief Create jog pad widget
 */
lv_obj_t* wgt_jog_pad_create(lv_obj_t* parent, jog_callback_t callback, void* user_data);

/**
 * @brief Set step selector values
 */
void wgt_jog_pad_set_steps(lv_obj_t* widget, const float* steps, size_t count);
