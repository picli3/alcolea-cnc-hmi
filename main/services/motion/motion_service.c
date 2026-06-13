/**
 * @file motion_service.c
 * @brief Motion service implementation (Phase 2)
 */

#include "motion_service.h"
#include "../grbl/grbl_manager.h"
#include "esp_log.h"

static const char* TAG = "motion_service";

esp_err_t motion_service_init(void)
{
    ESP_LOGI(TAG, "Motion service placeholder - Phase 2");
    return ESP_OK;
}

esp_err_t motion_jog(float x, float y, float z, float feed)
{
    char cmd[64];
    snprintf(cmd, sizeof(cmd), "$J=X%.3f Y%.3f Z%.3f F%.1f", x, y, z, feed);
    return grbl_send_command(cmd);
}

esp_err_t motion_jog_stop(void)
{
    return grbl_send_immediate("!");
}

esp_err_t motion_set_zero(uint8_t axis)
{
    return grbl_send_command("G10 L20 P0 X0 Y0 Z0");
}

esp_err_t motion_home_all(void)
{
    return grbl_send_command("$H");
}

esp_err_t motion_spindle_on(bool cw, uint16_t rpm)
{
    char cmd[32];
    snprintf(cmd, sizeof(cmd), "%s S%d", cw ? "M3 S" : "M4 S", rpm);
    return grbl_send_command(cmd);
}

esp_err_t motion_spindle_off(void)
{
    return grbl_send_command("M5");
}

esp_err_t motion_e_stop(void)
{
    grbl_send_immediate("!");
    vTaskDelay(pdMS_TO_TICKS(100));
    grbl_send_immediate("\x18"); /* Ctrl-X */
    return ESP_OK;
}

esp_err_t motion_reset(void)
{
    return grbl_send_immediate("\x18"); /* Ctrl-X */
}
