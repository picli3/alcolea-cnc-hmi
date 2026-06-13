/**
 * @file webserver.h
 * @brief Embedded HTTP server header (Phase 3)
 */

#pragma once

#include "esp_err.h"

/**
 * @brief Initialize HTTP server
 */
esp_err_t webserver_init(uint16_t port);

/**
 * @brief Start HTTP server
 */
esp_err_t webserver_start(void);

/**
 * @brief Stop HTTP server
 */
esp_err_t webserver_stop(void);
