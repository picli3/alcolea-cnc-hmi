/**
 * @file gcode_preview.h
 * @brief G-code preview header (Phase 3)
 */

#pragma once

#include "lvgl/lvgl.h"
#include "gcode_parser.h"

/**
 * @brief Create preview canvas
 */
lv_obj_t* gcode_preview_create(lv_obj_t* parent);

/**
 * @brief Draw preview from G-code job
 */
void gcode_preview_draw(lv_obj_t* canvas, gcode_preview_t* preview);
