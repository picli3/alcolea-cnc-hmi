/**
 * @file file_service.c
 * @brief File service implementation (Phase 3)
 */

#include "file_service.h"
#include "esp_log.h"

static const char* TAG = "file_service";

esp_err_t file_service_init(void)
{
    ESP_LOGI(TAG, "File service placeholder - Phase 3");
    return ESP_OK;
}

esp_err_t file_list(const char* dir, file_entry_t** entries, int* count)
{
    *entries = NULL;
    *count = 0;
    return ESP_OK;
}

esp_err_t file_load(const char* path, gcode_job_t* job)
{
    return ESP_OK;
}

esp_err_t file_delete(const char* path)
{
    return ESP_OK;
}

gcode_preview_t file_get_preview(gcode_job_t* job)
{
    gcode_preview_t preview = {0};
    return preview;
}
