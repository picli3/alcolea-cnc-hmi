/**
 * @file grbl_parser.h
 * @brief GRBL response parser
 */

#pragma once

#include "grbl_manager.h"

/**
 * @brief Parse GRBL status response
 *
 * Format: <Status|MPos:X,Y,Z|WPos:X,Y,Z|Buf:RX>
 */
esp_err_t grbl_parser_parse_status(const char* response, grbl_status_data_t* status);

/**
 * @brief Parse GRBL ok response
 */
bool grbl_parser_is_ok(const char* response);

/**
 * @brief Parse GRBL error response
 */
int grbl_parser_get_error_code(const char* response);

/**
 * @brief Parse GRBL message response
 */
esp_err_t grbl_parser_parse_message(const char* response, char* message, size_t max_len);
