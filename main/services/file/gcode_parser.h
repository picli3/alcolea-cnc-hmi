/**
 * @file gcode_parser.h
 * @brief G-code parser header (Phase 3)
 */

#pragma once

#include "esp_err.h"

/**
 * @brief Parse a single G-code line
 */
esp_err_t gcode_parse_line(const char* line, float* x, float* y, float* z, float* feed);

/**
 * @brief Check if line is a motion command
 */
bool gcode_is_motion_command(const char* line);
