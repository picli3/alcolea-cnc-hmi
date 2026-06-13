/**
 * @file motion_service.h
 * @brief Motion service header (Phase 2)
 *
 * Handles jogging, homing, and spindle control
 */

#pragma once

#include "esp_err.h"

/**
 * @brief Initialize motion service
 */
esp_err_t motion_service_init(void);

/**
 * @brief Jog move
 */
esp_err_t motion_jog(float x, float y, float z, float feed);

/**
 * @brief Stop jog
 */
esp_err_t motion_jog_stop(void);

/**
 * @brief Set work zero for axis
 */
esp_err_t motion_set_zero(uint8_t axis);

/**
 * @brief Home all axes
 */
esp_err_t motion_home_all(void);

/**
 * @brief Spindle on (CW)
 */
esp_err_t motion_spindle_on(bool cw, uint16_t rpm);

/**
 * @brief Spindle off
 */
esp_err_t motion_spindle_off(void);

/**
 * @brief Emergency stop
 */
esp_err_t motion_e_stop(void);

/**
 * @brief Reset GRBL
 */
esp_err_t motion_reset(void);
