/**
 * @file file_service.h
 * @brief File service header (Phase 3)
 *
 * Manages G-code files in SPIFFS
 */

#pragma once

#include "esp_err.h"
#include <stddef.h>

/**
 * @brief File entry structure
 */
typedef struct {
    char name[64];
    size_t size;
    time_t modified;
} file_entry_t;

/**
 * @brief G-code job structure
 */
typedef struct {
    char path[128];
    size_t total_lines;
    size_t processed_lines;
} gcode_job_t;

/**
 * @brief G-code preview data
 */
typedef struct {
    float min_x, max_x;
    float min_y, max_y;
    float min_z, max_z;
    size_t line_count;
} gcode_preview_t;

/**
 * @brief Initialize file service
 */
esp_err_t file_service_init(void);

/**
 * @brief List files in directory
 */
esp_err_t file_list(const char* dir, file_entry_t** entries, int* count);

/**
 * @brief Load G-code file
 */
esp_err_t file_load(const char* path, gcode_job_t* job);

/**
 * @brief Delete file
 */
esp_err_t file_delete(const char* path);

/**
 * @brief Get preview data
 */
gcode_preview_t file_get_preview(gcode_job_t* job);
