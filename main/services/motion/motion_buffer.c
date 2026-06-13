/**
 * @file motion_buffer.c
 * @brief Motion buffer implementation (Phase 4)
 */

#include "motion_buffer.h"
#include "esp_log.h"

static const char* TAG = "motion_buffer";

esp_err_t motion_buffer_init(size_t capacity)
{
    ESP_LOGI(TAG, "Motion buffer placeholder - Phase 4");
    return ESP_OK;
}

esp_err_t motion_buffer_load(gcode_job_t* job)
{
    return ESP_OK;
}

size_t motion_buffer_pending(void)
{
    return 0;
}

float motion_buffer_progress(void)
{
    return 0.0f;
}

void motion_buffer_clear(void)
{
}
