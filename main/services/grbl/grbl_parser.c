/**
 * @file grbl_parser.c
 * @brief GRBL response parser implementation
 */

#include "grbl_parser.h"
#include "esp_log.h"
#include "esp_err.h"
#include <string.h>
#include <stdlib.h>

static const char* TAG = "grbl_parser";

/**
 * @brief Extract float value from key:value pair
 */
static float parse_float(const char* str, const char* end)
{
    char buf[32];
    size_t len = (end && end > str) ? (end - str) : sizeof(buf) - 1;
    if (len >= sizeof(buf)) len = sizeof(buf) - 1;

    strncpy(buf, str, len);
    buf[len] = '\0';

    return atof(buf);
}

/**
 * @brief Parse position string (X:value,Y:value,Z:value)
 */
static void parse_position(const char* str, grbl_position_t* pos)
{
    const char* ptr = str;

    /* Parse X */
    const char* x_end = strchr(ptr, ',');
    if (x_end) {
        const char* x_val = strchr(ptr, ':');
        if (x_val && x_val < x_end) {
            pos->x = parse_float(x_val + 1, x_end);
        }
        ptr = x_end + 1;
    }

    /* Parse Y */
    const char* y_end = strchr(ptr, ',');
    if (y_end) {
        const char* y_val = strchr(ptr, ':');
        if (y_val && y_val < y_end) {
            pos->y = parse_float(y_val + 1, y_end);
        }
        ptr = y_end + 1;
    }

    /* Parse Z */
    const char* z_val = strchr(ptr, ':');
    if (z_val) {
        pos->z = parse_float(z_val + 1, NULL);
    }
}

/**
 * @brief Parse status type string
 */
static grbl_status_type_t parse_status_type(const char* str)
{
    if (strcmp(str, "Idle") == 0) return GRBL_STATUS_IDLE;
    if (strcmp(str, "Run") == 0) return GRBL_STATUS_RUN;
    if (strcmp(str, "Jog") == 0) return GRBL_STATUS_JOG;
    if (strcmp(str, "Hold") == 0) return GRBL_STATUS_HOLD;
    if (strcmp(str, "Alarm") == 0) return GRBL_STATUS_ALARM;
    if (strcmp(str, "Door") == 0) return GRBL_STATUS_DOOR;
    if (strcmp(str, "Check") == 0) return GRBL_STATUS_CHECK;
    if (strcmp(str, "Home") == 0) return GRBL_STATUS_HOME;
    if (strcmp(str, "Unlock") == 0) return GRBL_STATUS_UNLOCK;
    if (strcmp(str, "Off") == 0) return GRBL_STATUS_OFF;
    return GRBL_STATUS_UNKNOWN;
}

esp_err_t grbl_parser_parse_status(const char* response, grbl_status_data_t* status)
{
    if (!response || !status || response[0] != '<') {
        return ESP_ERR_INVALID_ARG;
    }

    memset(status, 0, sizeof(*status));

    const char* ptr = response + 1;
    const char* end = strchr(ptr, '>');
    if (!end) end = ptr + strlen(ptr);

    /* Parse status type */
    const char* sep = strchr(ptr, '|');
    char status_str[16];
    size_t status_len = sep ? (sep - ptr) : (end - ptr);
    if (status_len >= sizeof(status_str)) status_len = sizeof(status_str) - 1;

    strncpy(status_str, ptr, status_len);
    status_str[status_len] = '\0';
    status->status = parse_status_type(status_str);

    /* Parse fields */
    while (ptr < end) {
        sep = strchr(ptr, ':');
        if (!sep || sep > end) break;

        char key[16];
        size_t key_len = sep - ptr;
        if (key_len >= sizeof(key)) key_len = sizeof(key) - 1;
        strncpy(key, ptr, key_len);
        key[key_len] = '\0';

        const char* val_start = sep + 1;
        const char* next_sep = strchr(val_start, '|');
        if (!next_sep || next_sep > end) next_sep = end;

        if (strcmp(key, "MPos") == 0) {
            parse_position(val_start, &status->mpos);
        } else if (strcmp(key, "WPos") == 0) {
            parse_position(val_start, &status->wpos);
        } else if (strcmp(key, "WCO") == 0) {
            parse_position(val_start, &status->wco);
        } else if (strcmp(key, "Buf") == 0) {
            /* Parse buffer count */
            status->planner_buffer = atoi(val_start);
        } else if (strcmp(key, "RX") == 0) {
            /* Parse RX buffer */
            status->rx_buffer = atoi(val_start);
        }

        ptr = next_sep + 1;
    }

    return ESP_OK;
}

bool grbl_parser_is_ok(const char* response)
{
    return response && strcmp(response, "ok") == 0;
}

int grbl_parser_get_error_code(const char* response)
{
    if (!response) return -1;

    if (strncmp(response, "error:", 6) == 0) {
        return atoi(response + 6);
    }
    return -1;
}

esp_err_t grbl_parser_parse_message(const char* response, char* message, size_t max_len)
{
    if (!response || !message) {
        return ESP_ERR_INVALID_ARG;
    }

    if (strncmp(response, "[MSG:", 5) == 0) {
        const char* msg_start = response + 5;
        const char* msg_end = strchr(msg_start, ']');
        size_t msg_len = msg_end ? (msg_end - msg_start) : strlen(msg_start);

        if (msg_len >= max_len) msg_len = max_len - 1;
        strncpy(message, msg_start, msg_len);
        message[msg_len] = '\0';
        return ESP_OK;
    }

    return ESP_ERR_NOT_FOUND;
}
