/**
 * @file grbl_manager.h
 * @brief GRBL Manager - USB CDC-ACM communication with GRBL 1.1h
 */

#pragma once

#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

/**
 * @brief GRBL status type
 */
typedef enum {
    GRBL_STATUS_IDLE = 0,
    GRBL_STATUS_RUN,
    GRBL_STATUS_JOG,
    GRBL_STATUS_HOLD,
    GRBL_STATUS_ALARM,
    GRBL_STATUS_DOOR,
    GRBL_STATUS_CHECK,
    GRBL_STATUS_HOME,
    GRBL_STATUS_UNLOCK,
    GRBL_STATUS_OFF,
    GRBL_STATUS_UNKNOWN
} grbl_status_type_t;

/**
 * @brief GRBL position data
 */
typedef struct {
    float x;
    float y;
    float z;
} grbl_position_t;

/**
 * @brief GRBL status data
 */
typedef struct {
    grbl_status_type_t status;
    grbl_position_t mpos;      /* Machine position */
    grbl_position_t wpos;      /* Work position */
    grbl_position_t wco;        /* Work coordinate offset */
    uint16_t planner_buffer;
    uint16_t rx_buffer;
    uint16_t spindle_rpm;
    bool spindle_cw;
    bool coolant_mist;
    bool coolant_flood;
} grbl_status_data_t;

/**
 * @brief GRBL connection state
 */
typedef enum {
    GRBL_CONN_DISCONNECTED = 0,
    GRBL_CONN_CONNECTING,
    GRBL_CONN_CONNECTED,
    GRBL_CONN_ERROR
} grbl_conn_state_t;

/**
 * @brief GRBL status callback
 */
typedef void (*grbl_status_cb_t)(const grbl_status_data_t* status, void* user_data);

/**
 * @brief Initialize GRBL manager
 */
esp_err_t grbl_manager_init(void);

/**
 * @brief Send command to GRBL (blocks if buffer full)
 */
esp_err_t grbl_send_command(const char* cmd);

/**
 * @brief Send immediate command (ctrl commands)
 */
esp_err_t grbl_send_immediate(const char* cmd);

/**
 * @brief Check if GRBL is ready
 */
bool grbl_is_ready(void);

/**
 * @brief Check if GRBL is connected
 */
bool grbl_is_connected(void);

/**
 * @brief Get current GRBL status
 */
grbl_status_data_t grbl_get_status(void);

/**
 * @brief Register status callback
 */
esp_err_t grbl_register_status_callback(grbl_status_cb_t callback, void* user_data);

/**
 * @brief Get connection state
 */
grbl_conn_state_t grbl_get_connection_state(void);
