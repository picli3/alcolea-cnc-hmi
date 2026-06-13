/**
 * @file gcode_parser.c
 * @brief G-code parser implementation (Phase 3)
 */

#include "gcode_parser.h"
#include "esp_log.h"
#include <string.h>
#include <stdlib.h>

static const char* TAG = "gcode_parser";

esp_err_t gcode_parse_line(const char* line, float* x, float* y, float* z, float* feed)
{
    if (!line || !x || !y || !z) return ESP_ERR_INVALID_ARG;

    /* TODO: Implement G-code parsing */
    *x = *y = *z = *feed = 0.0f;
    return ESP_OK;
}

bool gcode_is_motion_command(const char* line)
{
    if (!line) return false;
    return (line[0] == 'G' && (line[1] == '0' || line[1] == '1' || line[1] == '2' || line[1] == '3'));
}
