/**
 * @file motion_buffer.h
 * @brief Motion buffer header (Phase 4)
 *
 * Ring buffer for G-code streaming
 */

#pragma once

#include "esp_err.h"
#include "file_service.h"

/**
 * @brief Initialize motion buffer
 */
esp_err_t motion_buffer_init(size_t capacity);

/**
 * @brief Load G-code job into buffer
 */
esp_err_t motion_buffer_load(gcode_job_t* job);

/**
 * @brief Get pending line count
 */
size_t motion_buffer_pending(void);

/**
 * @brief Get progress (0.0 - 1.0)
 */
float motion_buffer_progress(void);

/**
 * @brief Clear buffer
 */
void motion_buffer_clear(void);
