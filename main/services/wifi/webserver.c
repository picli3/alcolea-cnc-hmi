/**
 * @file webserver.c
 * @brief Embedded HTTP server implementation (Phase 3)
 */

#include "webserver.h"
#include "esp_log.h"

static const char* TAG = "webserver";

esp_err_t webserver_init(uint16_t port)
{
    ESP_LOGI(TAG, "HTTP server placeholder on port %d", port);
    return ESP_OK;
}

esp_err_t webserver_start(void)
{
    return ESP_OK;
}

esp_err_t webserver_stop(void)
{
    return ESP_OK;
}
