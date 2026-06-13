/**
 * @file cnc_state_machine.h
 * @brief CNC State Machine header
 */

#pragma once

#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/**
 * @brief CNC system states
 */
typedef enum {
    CNC_STATE_BOOT = 0,
    CNC_STATE_IDLE,
    CNC_STATE_CONNECTED,
    CNC_STATE_RUNNING,
    CNC_STATE_PAUSED,
    CNC_STATE_ALARM,
    CNC_STATE_HOLD,
    CNC_STATE_ERROR,
    CNC_STATE_COUNT
} cnc_state_t;

/**
 * @brief State machine transition result
 */
typedef struct {
    cnc_state_t from_state;
    cnc_state_t to_state;
    bool success;
} cnc_state_transition_t;

/**
 * @brief Initialize state machine
 */
esp_err_t cnc_state_machine_init(void);

/**
 * @brief Get current state
 */
cnc_state_t cnc_state_machine_get_state(void);

/**
 * @brief Request state transition
 */
esp_err_t cnc_state_machine_transition(cnc_state_t new_state);

/**
 * @brief State machine task
 */
void cnc_state_machine_task(void* pvParameters);

/**
 * @brief Get state name string
 */
const char* cnc_state_get_name(cnc_state_t state);
