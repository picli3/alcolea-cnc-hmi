/**
 * @file app_events.h
 * @brief Application event definitions
 */

#pragma once

#include <stdint.h>

/**
 * @brief Application event types
 */
typedef enum {
    EVT_SYSTEM_BOOT_COMPLETE,
    EVT_WIFI_LINK_UP,
    EVT_WIFI_LINK_DOWN,
    EVT_GRBL_CONNECTED,
    EVT_GRBL_DISCONNECTED,
    EVT_GRBL_STATUS_UPDATE,
    EVT_GCODE_FILE_LOADED,
    EVT_GCODE_STREAM_START,
    EVT_GCODE_STREAM_PAUSE,
    EVT_GCODE_STREAM_RESUME,
    EVT_GCODE_STREAM_COMPLETE,
    EVT_GCODE_STREAM_ABORT,
    EVT_JOG_START,
    EVT_JOG_STOP,
    EVT_ZERO_SET,
    EVT_HOME_CYCLE,
    EVT_ALARM_TRIGGERED,
    EVT_ALARM_CLEARED,
    EVT_ERROR_OCCURRED,
    EVT_TOUCH_INPUT,
} cnc_event_t;

/**
 * @brief Application event data
 */
typedef struct {
    cnc_event_t type;
    void* data;
    size_t len;
} cnc_event_data_t;

/**
 * @brief Event callback function type
 */
typedef void (*cnc_event_cb_t)(cnc_event_t event, const cnc_event_data_t* data, void* user_data);

/**
 * @brief Register event callback
 */
esp_err_t app_events_register(cnc_event_cb_t callback, void* user_data);

/**
 * @brief Unregister event callback
 */
esp_err_t app_events_unregister(cnc_event_cb_t callback);

/**
 * @brief Post an event to the application event queue
 */
esp_err_t app_events_post(cnc_event_t event, const void* data, size_t len);
