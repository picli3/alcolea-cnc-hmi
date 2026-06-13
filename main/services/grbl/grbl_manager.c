/**
 * @file grbl_manager.c
 * @brief GRBL Manager implementation
 */

#include "grbl_manager.h"
#include "grbl_parser.h"
#include "main.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/usb_serial_jtag.h"
#include "hal/usb_serial_jtag_ll.h"

static const char* TAG = "grbl_mgr";

/* Task configuration */
#define GRBL_TASK_STACK_SIZE    4096
#define GRBL_TASK_PRIORITY      5
#define GRBL_STATUS_QUERY_MS    100

/* USB CDC buffer size */
#define USB_CDC_RX_BUFFER_SIZE  512
#define USB_CDC_TX_BUFFER_SIZE  512

/* GRBL command queue */
#define GRBL_CMD_QUEUE_SIZE     32

static TaskHandle_t s_grbl_task_handle;
static StaticTask_t s_grbl_task_buffer;
static StackType_t s_grbl_task_stack[GRBL_TASK_STACK_SIZE];

static QueueHandle_t s_cmd_queue;
static StaticQueue_t s_cmd_queue_buffer;
static uint8_t s_cmd_queue_storage[GRBL_CMD_QUEUE_SIZE * 64];

static grbl_status_data_t s_grbl_status;
static grbl_conn_state_t s_conn_state = GRBL_CONN_DISCONNECTED;
static grbl_status_cb_t s_status_callback;
static void* s_status_user_data;

static usb_serial_jtag_dev_t* s_usb_dev;
static char s_rx_buffer[USB_CDC_RX_BUFFER_SIZE];
static size_t s_rx_len;

/**
 * @brief Parse GRBL status response
 */
static void parse_status_response(const char* response)
{
    grbl_parser_parse_status(response, &s_grbl_status);
}

/**
 * @brief GRBL manager task
 */
static void grbl_manager_task(void* pvParameters)
{
    TickType_t last_wake = xTaskGetTickCount();
    int64_t last_status_query = 0;

    ESP_LOGI(TAG, "GRBL manager task started");

    while (1) {
        int64_t now = esp_timer_get_time() / 1000;

        /* Check for USB CDC connection */
        if (s_conn_state == GRBL_CONN_DISCONNECTED) {
            /* Try to detect USB connection */
            s_conn_state = GRBL_CONN_CONNECTING;
            ESP_LOGI(TAG, "Attempting USB CDC connection...");
        }

        /* Query GRBL status periodically */
        if (now - last_status_query > GRBL_STATUS_QUERY_MS) {
            if (s_conn_state == GRBL_CONN_CONNECTED) {
                grbl_send_command("?");
            }
            last_status_query = now;
        }

        /* Process received data */
        int bytes_read = usb_serial_jtag_read_bytes(
            s_rx_buffer, USB_CDC_RX_BUFFER_SIZE - 1, 0);
        if (bytes_read > 0) {
            s_rx_buffer[bytes_read] = '\0';
            ESP_LOGD(TAG, "USB RX: %.*s", bytes_read, s_rx_buffer);

            /* Parse GRBL responses */
            char* line_start = s_rx_buffer;
            for (int i = 0; i < bytes_read; i++) {
                if (s_rx_buffer[i] == '\n' || s_rx_buffer[i] == '\r') {
                    s_rx_buffer[i] = '\0';
                    if (*line_start) {
                        /* Check if status response */
                        if (line_start[0] == '<') {
                            parse_status_response(line_start);
                            if (s_status_callback) {
                                s_status_callback(&s_grbl_status, s_status_user_data);
                            }
                        } else {
                            /* Log other responses */
                            ESP_LOGD(TAG, "GRBL: %s", line_start);
                        }
                    }
                    line_start = &s_rx_buffer[i + 1];
                }
            }
        }

        /* Process command queue */
        char cmd[64];
        if (xQueueReceive(s_cmd_queue, cmd, 0) == pdTRUE) {
            size_t len = strlen(cmd);
            if (len > 0) {
                /* Add newline if not present */
                if (cmd[len - 1] != '\n') {
                    cmd[len++] = '\n';
                    cmd[len] = '\0';
                }
                usb_serial_jtag_write_bytes(cmd, len, portMAX_DELAY);
                ESP_LOGD(TAG, "USB TX: %s", cmd);
            }
        }

        vTaskDelayUntil(&last_wake, pdMS_TO_TICKS(10));
    }
}

esp_err_t grbl_manager_init(void)
{
    ESP_LOGI(TAG, "Initializing GRBL manager");

    /* Initialize USB CDC */
    s_usb_dev = &USB_SERIAL_JTAG_DEV;
    usb_serial_jtag_config_t config = {
        .tx_buffer_size = USB_CDC_TX_BUFFER_SIZE,
        .rx_buffer_size = USB_CDC_RX_BUFFER_SIZE,
    };
    esp_err_t ret = usb_serial_jtag_init();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "USB CDC init failed: %s", esp_err_to_name(ret));
        return ret;
    }

    /* Create command queue */
    s_cmd_queue = xQueueCreateStatic(
        GRBL_CMD_QUEUE_SIZE,
        64,
        s_cmd_queue_storage,
        &s_cmd_queue_buffer
    );

    /* Initialize status */
    memset(&s_grbl_status, 0, sizeof(s_grbl_status));
    s_grbl_status.status = GRBL_STATUS_UNKNOWN;

    /* Create GRBL manager task */
    s_grbl_task_handle = xTaskCreateStaticPinnedToCore(
        grbl_manager_task,
        "grbl_mgr",
        GRBL_TASK_STACK_SIZE,
        NULL,
        GRBL_TASK_PRIORITY,
        s_grbl_task_stack,
        &s_grbl_task_buffer,
        0
    );

    if (s_grbl_task_handle == NULL) {
        ESP_LOGE(TAG, "Failed to create GRBL task");
        return ESP_ERR_NO_MEM;
    }

    s_conn_state = GRBL_CONN_CONNECTING;

    ESP_LOGI(TAG, "GRBL manager initialized");
    return ESP_OK;
}

esp_err_t grbl_send_command(const char* cmd)
{
    if (s_conn_state != GRBL_CONN_CONNECTED) {
        return ESP_ERR_INVALID_STATE;
    }
    if (xQueueSend(s_cmd_queue, cmd, pdMS_TO_TICKS(1000)) != pdTRUE) {
        ESP_LOGE(TAG, "Command queue full");
        return ESP_ERR_TIMEOUT;
    }
    return ESP_OK;
}

esp_err_t grbl_send_immediate(const char* cmd)
{
    if (s_conn_state != GRBL_CONN_CONNECTED) {
        return ESP_ERR_INVALID_STATE;
    }

    size_t len = strlen(cmd);
    char cmd_nl[64];
    if (len >= sizeof(cmd_nl) - 1) {
        return ESP_ERR_INVALID_ARG;
    }
    memcpy(cmd_nl, cmd, len);
    if (cmd_nl[len - 1] != '\n') {
        cmd_nl[len++] = '\n';
    }
    cmd_nl[len] = '\0';

    usb_serial_jtag_write_bytes(cmd_nl, len, portMAX_DELAY);
    ESP_LOGD(TAG, "USB TX (immediate): %s", cmd_nl);
    return ESP_OK;
}

bool grbl_is_ready(void)
{
    return s_conn_state == GRBL_CONN_CONNECTED &&
           s_grbl_status.status != GRBL_STATUS_UNKNOWN;
}

bool grbl_is_connected(void)
{
    return s_conn_state == GRBL_CONN_CONNECTED;
}

grbl_status_data_t grbl_get_status(void)
{
    return s_grbl_status;
}

esp_err_t grbl_register_status_callback(grbl_status_cb_t callback, void* user_data)
{
    s_status_callback = callback;
    s_status_user_data = user_data;
    return ESP_OK;
}

grbl_conn_state_t grbl_get_connection_state(void)
{
    return s_conn_state;
}
