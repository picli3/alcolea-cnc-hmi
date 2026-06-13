/**
 * @file main.h
 * @brief CNC HMI Main Header
 * @version 1.0
 * @date 2026-06-13
 */

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_err.h"
#include "nvs_flash.h"
#include "spiffs/spiffs.h"

#include "app/app.h"
#include "sm/cnc_state_machine.h"
#include "ui/ui.h"
#include "services/grbl/grbl_manager.h"

#define TAG "CNC_HMI"

/* Application version */
#define APP_VERSION "1.0.0"
#define APP_NAME "Alcolea CNC HMI"

/* System event bits */
#define SYSTEM_BOOT_BIT      (1 << 0)
#define GRBL_CONNECTED_BIT   (1 << 1)
#define GRBL_READY_BIT       (1 << 2)
#define UI_READY_BIT         (1 << 3)
#define WIFI_LINK_UP_BIT     (1 << 4)

/* Application event group */
extern EventGroupHandle_t s_system_event_group;

/* Task priorities */
#define GRBL_MANAGER_TASK_PRIORITY    5
#define STATE_MACHINE_TASK_PRIORITY  4
#define UI_TASK_PRIORITY             3

/* Task stack sizes */
#define GRBL_MANAGER_TASK_STACK      4096
#define STATE_MACHINE_TASK_STACK     4096
#define UI_TASK_STACK                8192

/**
 * @brief Application initialization
 */
esp_err_t app_init(void);

/**
 * @brief Application main loop
 */
void app_main_loop(void* pvParameters);
