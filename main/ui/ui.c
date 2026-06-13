/**
 * @file ui.c
 * @brief LVGL UI Layer implementation
 */

#include "ui.h"
#include "ui/styles/theme_alcolea.h"
#include "screens/scr_home.h"
#include "grbl_manager.h"
#include "cnc_state_machine.h"
#include "esp_log.h"
#include "esp_timer.h"

static const char* TAG = "ui";

/* Display configuration */
#define UI_DISPLAY_WIDTH    1024
#define UI_DISPLAY_HEIGHT   600
#define UI_LVGL_TASK_PRIORITY   2

static TaskHandle_t s_lvgl_task_handle;
static StaticTask_t s_lvgl_task_buffer;
static StackType_t s_lvgl_task_stack[8192];

static lv_disp_t* s_disp;
static lv_obj_t* s_status_bar;
static lv_obj_t* s_pos_label;
static lv_obj_t* s_state_label;
static lv_obj_t* s_conn_label;

/* Theme colors */
#define COLOR_BG_DARK       0x1A1A2E
#define COLOR_PRIMARY       0x16213E
#define COLOR_ACCENT        0x0F3460
#define COLOR_CYAN           0x00D9FF
#define COLOR_TEXT          0xFFFFFF
#define COLOR_TEXT_DIM      0x808080
#define COLOR_RED           0xFF4444
#define COLOR_GREEN         0x44FF44
#define COLOR_YELLOW        0xFFFF44

/**
 * @brief Initialize display driver
 */
static esp_err_t ui_display_init(void)
{
    ESP_LOGI(TAG, "Initializing display");

    /* TODO: Initialize RGB display for ESP32-P4 */
    /* For now, use dummy display for compilation */

    ESP_LOGI(TAG, "Display initialized (1024x600)");
    return ESP_OK;
}

/**
 * @brief Flush callback for LVGL
 */
static void ui_flush_cb(lv_disp_t* disp, const lv_area_t* area, lv_color_t* color_p)
{
    /* TODO: Implement actual display flush for RGB panel */
    lv_disp_flush_ready(disp);
}

/**
 * @brief Read touch callback
 */
static void ui_read_touch_cb(lv_indev_t* indev, lv_indev_data_t* data)
{
    /* TODO: Implement touch reading */
    data->state = LV_INDEV_STATE_REL;
}

/**
 * @brief LVGL task
 */
static void lvgl_task(void* pvParameters)
{
    ESP_LOGI(TAG, "LVGL task started");

    TickType_t last_wake = xTaskGetTickCount();

    while (1) {
        lv_task_handler();
        vTaskDelayUntil(&last_wake, pdMS_TO_TICKS(5));
    }
}

esp_err_t ui_init(void)
{
    ESP_LOGI(TAG, "Initializing LVGL UI");

    /* Initialize display */
    esp_err_t ret = ui_display_init();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Display init failed");
        return ret;
    }

    /* Initialize LVGL */
    lv_init();

    /* Register theme */
    theme_alcolea_init();

    /* Create display driver */
    static lv_disp_buf_t disp_buf;
    static lv_color_t buf[UI_DISPLAY_WIDTH * 32];
    lv_disp_buf_init(&disp_buf, buf, NULL, UI_DISPLAY_WIDTH * 32);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.buffer = &disp_buf;
    disp_drv.flush_cb = ui_flush_cb;
    disp_drv.hor_res = UI_DISPLAY_WIDTH;
    disp_drv.ver_res = UI_DISPLAY_HEIGHT;
    s_disp = lv_disp_register(&disp_drv);

    /* Create touch input device */
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = ui_read_touch_cb;
    lv_indev_register(&indev_drv);

    /* Create home screen */
    scr_home_create();

    /* Create LVGL task */
    s_lvgl_task_handle = xTaskCreateStaticPinnedToCore(
        lvgl_task,
        "lvgl",
        8192,
        NULL,
        UI_LVGL_TASK_PRIORITY,
        s_lvgl_task_stack,
        &s_lvgl_task_buffer,
        1
    );

    if (s_lvgl_task_handle == NULL) {
        ESP_LOGE(TAG, "Failed to create LVGL task");
        return ESP_ERR_NO_MEM;
    }

    /* Register GRBL status callback */
    grbl_register_status_callback(ui_on_grbl_status, NULL);

    ESP_LOGI(TAG, "UI initialized");
    return ESP_OK;
}

void ui_tick(void)
{
    /* LVGL tick is handled by lvgl_task */
}

void ui_update_status(const char* status_str, uint16_t planner_buf, uint16_t rx_buf)
{
    if (s_state_label) {
        lv_label_set_text(s_state_label, status_str);
    }
}

void ui_update_position(float x, float y, float z, bool is_work)
{
    if (s_pos_label) {
        char buf[64];
        snprintf(buf, sizeof(buf), "X:%.3f Y:%.3f Z:%.3f", x, y, z);
        lv_label_set_text(s_pos_label, buf);
    }
}

void ui_update_connection(bool connected)
{
    if (s_conn_label) {
        if (connected) {
            lv_label_set_text(s_conn_label, "Conectado");
            lv_obj_set_style_text_color(s_conn_label, lv_color_hex(COLOR_GREEN), 0);
        } else {
            lv_label_set_text(s_conn_label, "Desconectado");
            lv_obj_set_style_text_color(s_conn_label, lv_color_hex(COLOR_RED), 0);
        }
    }
}

int ui_get_width(void)
{
    return UI_DISPLAY_WIDTH;
}

int ui_get_height(void)
{
    return UI_DISPLAY_HEIGHT;
}

/**
 * @brief GRBL status callback
 */
void ui_on_grbl_status(const grbl_status_data_t* status, void* user_data)
{
    static const char* status_names[] = {
        "Idle", "Run", "Jog", "Hold", "Alarm", "Door", "Check", "Home", "Unlock", "Off"
    };

    const char* status_str = (status->status < 10) ?
        status_names[status->status] : "Unknown";

    ui_update_status(status_str, status->planner_buffer, status->rx_buffer);
    ui_update_position(status->wpos.x, status->wpos.y, status->wpos.z, true);
}
