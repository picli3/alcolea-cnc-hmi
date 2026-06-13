/**
 * @file cnc_state_machine.c
 * @brief CNC State Machine implementation
 */

#include "cnc_state_machine.h"
#include "main.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

static const char* TAG = "sm";

static cnc_state_t s_current_state = CNC_STATE_BOOT;
static TaskHandle_t s_sm_task_handle;
static StaticTask_t s_sm_task_buffer;
static StackType_t s_sm_task_stack[4096];

/**
 * @brief State entry handlers
 */
static esp_err_t state_on_enter(cnc_state_t state)
{
    switch (state) {
        case CNC_STATE_BOOT:
            ESP_LOGI(TAG, "Entering BOOT state");
            break;
        case CNC_STATE_IDLE:
            ESP_LOGI(TAG, "Entering IDLE state");
            break;
        case CNC_STATE_CONNECTED:
            ESP_LOGI(TAG, "Entering CONNECTED state");
            break;
        case CNC_STATE_RUNNING:
            ESP_LOGI(TAG, "Entering RUNNING state");
            break;
        case CNC_STATE_PAUSED:
            ESP_LOGI(TAG, "Entering PAUSED state");
            break;
        case CNC_STATE_ALARM:
            ESP_LOGI(TAG, "Entering ALARM state");
            break;
        case CNC_STATE_HOLD:
            ESP_LOGI(TAG, "Entering HOLD state");
            break;
        case CNC_STATE_ERROR:
            ESP_LOGE(TAG, "Entering ERROR state");
            break;
        default:
            break;
    }
    return ESP_OK;
}

/**
 * @brief State exit handlers
 */
static esp_err_t state_on_exit(cnc_state_t state)
{
    switch (state) {
        case CNC_STATE_BOOT:
            ESP_LOGI(TAG, "Exiting BOOT state");
            break;
        case CNC_STATE_IDLE:
            ESP_LOGI(TAG, "Exiting IDLE state");
            break;
        case CNC_STATE_CONNECTED:
            ESP_LOGI(TAG, "Exiting CONNECTED state");
            break;
        case CNC_STATE_RUNNING:
            ESP_LOGI(TAG, "Exiting RUNNING state");
            break;
        case CNC_STATE_PAUSED:
            ESP_LOGI(TAG, "Exiting PAUSED state");
            break;
        case CNC_STATE_ALARM:
            ESP_LOGI(TAG, "Exiting ALARM state");
            break;
        case CNC_STATE_HOLD:
            ESP_LOGI(TAG, "Exiting HOLD state");
            break;
        case CNC_STATE_ERROR:
            ESP_LOGI(TAG, "Exiting ERROR state");
            break;
        default:
            break;
    }
    return ESP_OK;
}

esp_err_t cnc_state_machine_init(void)
{
    ESP_LOGI(TAG, "Initializing state machine");

    s_current_state = CNC_STATE_BOOT;
    state_on_enter(CNC_STATE_BOOT);

    /* Create state machine task */
    s_sm_task_handle = xTaskCreateStaticPinnedToCore(
        cnc_state_machine_task,
        "sm_task",
        4096,
        NULL,
        4,
        s_sm_task_stack,
        &s_sm_task_buffer,
        0
    );

    if (s_sm_task_handle == NULL) {
        ESP_LOGE(TAG, "Failed to create state machine task");
        return ESP_ERR_NO_MEM;
    }

    ESP_LOGI(TAG, "State machine initialized");
    return ESP_OK;
}

cnc_state_t cnc_state_machine_get_state(void)
{
    return s_current_state;
}

esp_err_t cnc_state_machine_transition(cnc_state_t new_state)
{
    if (new_state == s_current_state) {
        return ESP_OK;
    }

    if (new_state >= CNC_STATE_COUNT) {
        ESP_LOGE(TAG, "Invalid state: %d", new_state);
        return ESP_ERR_INVALID_ARG;
    }

    ESP_LOGI(TAG, "State transition: %s -> %s",
             cnc_state_get_name(s_current_state),
             cnc_state_get_name(new_state));

    state_on_exit(s_current_state);
    s_current_state = new_state;
    state_on_enter(new_state);

    return ESP_OK;
}

void cnc_state_machine_task(void* pvParameters)
{
    EventBits_t bits;
    const TickType_t xDelay = pdMS_TO_TICKS(100);

    ESP_LOGI(TAG, "State machine task started");

    while (1) {
        bits = xEventGroupWaitBits(
            s_system_event_group,
            SYSTEM_BOOT_BIT,
            pdFALSE,
            pdFALSE,
            xDelay
        );

        if (bits & SYSTEM_BOOT_BIT) {
            /* Boot complete - transition to IDLE */
            if (s_current_state == CNC_STATE_BOOT) {
                cnc_state_machine_transition(CNC_STATE_IDLE);
            }
        }

        /* Process state-specific logic */
        switch (s_current_state) {
            case CNC_STATE_IDLE:
                /* Check for GRBL connection */
                if (grbl_is_connected()) {
                    cnc_state_machine_transition(CNC_STATE_CONNECTED);
                }
                break;
            case CNC_STATE_CONNECTED:
                /* Check for disconnection */
                if (!grbl_is_connected()) {
                    cnc_state_machine_transition(CNC_STATE_IDLE);
                }
                break;
            default:
                break;
        }
    }
}

const char* cnc_state_get_name(cnc_state_t state)
{
    static const char* state_names[] = {
        "BOOT",
        "IDLE",
        "CONNECTED",
        "RUNNING",
        "PAUSED",
        "ALARM",
        "HOLD",
        "ERROR"
    };

    if (state >= CNC_STATE_COUNT) {
        return "UNKNOWN";
    }
    return state_names[state];
}
