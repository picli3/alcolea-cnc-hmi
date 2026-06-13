/**
 * @file scr_home.c
 * @brief Home screen implementation
 *
 * Layout based on design reference:
 * - Top status bar: X/Y/Z coordinates, GRBL status, buffer usage
 * - Center: 2D toolpath preview (placeholder)
 * - Right panel: Jog pad, sliders, control buttons
 * - Bottom: Emergency stop button
 */

#include "scr_home.h"
#include "../styles/theme_alcolea.h"
#include "cnc_state_machine.h"
#include "esp_log.h"

static const char* TAG = "scr_home";

/* Screen dimensions */
#define STATUS_BAR_HEIGHT   60
#define SIDEBAR_WIDTH       280
#define PREVIEW_MARGIN      16
#define BUTTON_HEIGHT       48
#define JOG_BUTTON_SIZE     64

/* Colors */
#define C_BG_DARK       0x1A1A2E
#define C_PANEL         0x16213E
#define C_ACCENT        0x00D9FF
#define C_RED           0xFF4444
#define C_GREEN         0x44FF44
#define C_YELLOW        0xFFFF44
#define C_WHITE         0xFFFFFF
#define C_DIM           0x808080

static lv_obj_t* s_screen;
static lv_obj_t* s_status_bar;
static lv_obj_t* s_pos_x_label;
static lv_obj_t* s_pos_y_label;
static lv_obj_t* s_pos_z_label;
static lv_obj_t* s_grbl_status_label;
static lv_obj_t* s_buffer_label;
static lv_obj_t* s_conn_label;
static lv_obj_t* s_preview_canvas;
static lv_obj_t* s_jog_pad_x;
static lv_obj_t* s_jog_pad_y;
static lv_obj_t* s_jog_pad_z;
static lv_obj_t* s_step_selector;
static lv_obj_t* s_feed_slider;
static lv_obj_t* s_spindle_slider;
static lv_obj_t* s_btn_start;
static lv_obj_t* s_btn_pause;
static lv_obj_t* s_btn_stop;
static lv_obj_t* s_btn_home;
static lv_obj_t* s_btn_zero;
static lv_obj_t* s_btn_estop;

/* Step values for jog */
static const float s_jog_steps[] = {0.1f, 1.0f, 10.0f, 50.0f};
static int s_jog_step_index = 1; /* Default 1mm */
static float s_feed_rate = 1000.0f;
static float s_spindle_rpm = 0.0f;

/**
 * @brief Create status bar at top
 */
static void create_status_bar(lv_obj_t* parent)
{
    s_status_bar = lv_obj_create(parent);
    lv_obj_set_size(s_status_bar, lv_obj_get_width(parent), STATUS_BAR_HEIGHT);
    lv_obj_set_pos(s_status_bar, 0, 0);
    lv_obj_set_style_bg_color(s_status_bar, lv_color_hex(C_PANEL), 0);
    lv_obj_set_style_border_width(s_status_bar, 0, 0);
    lv_obj_set_style_radius(s_status_bar, 0, 0);

    /* Connection indicator */
    s_conn_label = lv_label_create(s_status_bar);
    lv_label_set_text(s_conn_label, "Desconectado");
    lv_obj_set_pos(s_conn_label, 16, 20);
    lv_obj_set_style_text_color(s_conn_label, lv_color_hex(C_RED), 0);
    lv_obj_set_style_text_font(s_conn_label, &lv_font_montserrat_16, 0);

    /* GRBL Status */
    s_grbl_status_label = lv_label_create(s_status_bar);
    lv_label_set_text(s_grbl_status_label, "---");
    lv_obj_set_pos(s_grbl_status_label, 150, 20);
    lv_obj_set_style_text_color(s_grbl_status_label, lv_color_hex(C_YELLOW), 0);
    lv_obj_set_style_text_font(s_grbl_status_label, &lv_font_montserrat_16, 0);

    /* Position X */
    s_pos_x_label = lv_label_create(s_status_bar);
    lv_label_set_text(s_pos_x_label, "X: 0.000");
    lv_obj_set_pos(s_pos_x_label, 300, 10);
    lv_obj_set_style_text_color(s_pos_x_label, lv_color_hex(C_ACCENT), 0);
    lv_obj_set_style_text_font(s_pos_x_label, &lv_font_montserrat_14, 0);

    /* Position Y */
    s_pos_y_label = lv_label_create(s_status_bar);
    lv_label_set_text(s_pos_y_label, "Y: 0.000");
    lv_obj_set_pos(s_pos_y_label, 420, 10);
    lv_obj_set_style_text_color(s_pos_y_label, lv_color_hex(C_ACCENT), 0);
    lv_obj_set_style_text_font(s_pos_y_label, &lv_font_montserrat_14, 0);

    /* Position Z */
    s_pos_z_label = lv_label_create(s_status_bar);
    lv_label_set_text(s_pos_z_label, "Z: 0.000");
    lv_obj_set_pos(s_pos_z_label, 540, 10);
    lv_obj_set_style_text_color(s_pos_z_label, lv_color_hex(C_ACCENT), 0);
    lv_obj_set_style_text_font(s_pos_z_label, &lv_font_montserrat_14, 0);

    /* Buffer usage */
    s_buffer_label = lv_label_create(s_status_bar);
    lv_label_set_text(s_buffer_label, "Buf: 0/0");
    lv_obj_set_pos(s_buffer_label, 680, 20);
    lv_obj_set_style_text_color(s_buffer_label, lv_color_hex(C_DIM), 0);
    lv_obj_set_style_text_font(s_buffer_label, &lv_font_montserrat_14, 0);
}

/**
 * @brief Create 2D preview canvas (center area)
 */
static void create_preview_area(lv_obj_t* parent)
{
    int preview_x = SIDEBAR_WIDTH + PREVIEW_MARGIN;
    int preview_y = STATUS_BAR_HEIGHT + PREVIEW_MARGIN;
    int preview_w = lv_obj_get_width(parent) - SIDEBAR_WIDTH - PREVIEW_MARGIN * 2;
    int preview_h = lv_obj_get_height(parent) - STATUS_BAR_HEIGHT - BUTTON_HEIGHT - PREVIEW_MARGIN * 3;

    s_preview_canvas = lv_obj_create(parent);
    lv_obj_set_size(s_preview_canvas, preview_w, preview_h);
    lv_obj_set_pos(s_preview_canvas, preview_x, preview_y);
    lv_obj_set_style_bg_color(s_preview_canvas, lv_color_hex(C_BG_DARK), 0);
    lv_obj_set_style_border_width(s_preview_canvas, 2, 0);
    lv_obj_set_style_border_color(s_preview_canvas, lv_color_hex(C_ACCENT), 0);

    /* Title label */
    lv_obj_t* title = lv_label_create(s_preview_canvas);
    lv_label_set_text(title, "Toolpath Preview");
    lv_obj_set_pos(title, 10, 10);
    lv_obj_set_style_text_color(title, lv_color_hex(C_DIM), 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_12, 0);

    /* Placeholder - coordinate cross */
    lv_obj_t* cross_v = lv_obj_create(s_preview_canvas);
    lv_obj_set_size(cross_v, 1, preview_h - 40);
    lv_obj_set_pos(cross_v, (preview_w - 1) / 2, 30);
    lv_obj_set_style_bg_color(cross_v, lv_color_hex(C_DIM), 0);
    lv_obj_set_style_border_width(cross_v, 0, 0);

    lv_obj_t* cross_h = lv_obj_create(s_preview_canvas);
    lv_obj_set_size(cross_h, preview_w - 40, 1);
    lv_obj_set_pos(cross_h, 20, (preview_h - 1) / 2);
    lv_obj_set_style_bg_color(cross_h, lv_color_hex(C_DIM), 0);
    lv_obj_set_style_border_width(cross_h, 0, 0);
}

/**
 * @brief Create jog pad buttons
 */
static void create_jog_pad(lv_obj_t* parent)
{
    /* X axis jog */
    s_jog_pad_x = lv_obj_create(parent);
    lv_obj_set_size(s_jog_pad_x, JOG_BUTTON_SIZE * 3 + 20, JOG_BUTTON_SIZE + 30);
    lv_obj_set_pos(s_jog_pad_x, 10, 10);
    lv_obj_set_style_bg_color(s_jog_pad_x, lv_color_hex(C_PANEL), 0);
    lv_obj_set_style_border_width(s_jog_pad_x, 1, 0);
    lv_obj_set_style_border_color(s_jog_pad_x, lv_color_hex(C_ACCENT), 0);

    lv_obj_t* x_label = lv_label_create(s_jog_pad_x);
    lv_label_set_text(x_label, "X");
    lv_obj_set_pos(x_label, 10, 5);
    lv_obj_set_style_text_color(x_label, lv_color_hex(C_ACCENT), 0);

    /* Y axis jog */
    s_jog_pad_y = lv_obj_create(parent);
    lv_obj_set_size(s_jog_pad_y, JOG_BUTTON_SIZE * 3 + 20, JOG_BUTTON_SIZE + 30);
    lv_obj_set_pos(s_jog_pad_y, 10, 110);
    lv_obj_set_style_bg_color(s_jog_pad_y, lv_color_hex(C_PANEL), 0);
    lv_obj_set_style_border_width(s_jog_pad_y, 1, 0);
    lv_obj_set_style_border_color(s_jog_pad_y, lv_color_hex(C_ACCENT), 0);

    lv_obj_t* y_label = lv_label_create(s_jog_pad_y);
    lv_label_set_text(y_label, "Y");
    lv_obj_set_pos(y_label, 10, 5);
    lv_obj_set_style_text_color(y_label, lv_color_hex(C_ACCENT), 0);

    /* Z axis jog */
    s_jog_pad_z = lv_obj_create(parent);
    lv_obj_set_size(s_jog_pad_z, JOG_BUTTON_SIZE * 3 + 20, JOG_BUTTON_SIZE + 30);
    lv_obj_set_pos(s_jog_pad_z, 10, 210);
    lv_obj_set_style_bg_color(s_jog_pad_z, lv_color_hex(C_PANEL), 0);
    lv_obj_set_style_border_width(s_jog_pad_z, 1, 0);
    lv_obj_set_style_border_color(s_jog_pad_z, lv_color_hex(C_ACCENT), 0);

    lv_obj_t* z_label = lv_label_create(s_jog_pad_z);
    lv_label_set_text(z_label, "Z");
    lv_obj_set_pos(z_label, 10, 5);
    lv_obj_set_style_text_color(z_label, lv_color_hex(C_ACCENT), 0);
}

/**
 * @brief Create step selector
 */
static void create_step_selector(lv_obj_t* parent)
{
    s_step_selector = lv_obj_create(parent);
    lv_obj_set_size(s_step_selector, SIDEBAR_WIDTH - 20, 50);
    lv_obj_set_pos(s_step_selector, 10, 320);
    lv_obj_set_style_bg_color(s_step_selector, lv_color_hex(C_PANEL), 0);
    lv_obj_set_style_border_width(s_step_selector, 1, 0);
    lv_obj_set_style_border_color(s_step_selector, lv_color_hex(C_ACCENT), 0);

    lv_obj_t* label = lv_label_create(s_step_selector);
    lv_label_set_text(label, "Step:");
    lv_obj_set_pos(label, 10, 15);
    lv_obj_set_style_text_color(label, lv_color_hex(C_DIM), 0);
}

/**
 * @brief Create sliders (feed and spindle)
 */
static void create_sliders(lv_obj_t* parent)
{
    /* Feed rate slider */
    s_feed_slider = lv_obj_create(parent);
    lv_obj_set_size(s_feed_slider, SIDEBAR_WIDTH - 20, 80);
    lv_obj_set_pos(s_feed_slider, 10, 380);
    lv_obj_set_style_bg_color(s_feed_slider, lv_color_hex(C_PANEL), 0);
    lv_obj_set_style_border_width(s_feed_slider, 1, 0);
    lv_obj_set_style_border_color(s_feed_slider, lv_color_hex(C_ACCENT), 0);

    lv_obj_t* feed_label = lv_label_create(s_feed_slider);
    lv_label_set_text(feed_label, "Feed: 1000 mm/min");
    lv_obj_set_pos(feed_label, 10, 10);
    lv_obj_set_style_text_color(feed_label, lv_color_hex(C_WHITE), 0);
    lv_obj_set_style_text_font(feed_label, &lv_font_montserrat_12, 0);

    /* Spindle slider */
    s_spindle_slider = lv_obj_create(parent);
    lv_obj_set_size(s_spindle_slider, SIDEBAR_WIDTH - 20, 80);
    lv_obj_set_pos(s_spindle_slider, 10, 470);
    lv_obj_set_style_bg_color(s_spindle_slider, lv_color_hex(C_PANEL), 0);
    lv_obj_set_style_border_width(s_spindle_slider, 1, 0);
    lv_obj_set_style_border_color(s_spindle_slider, lv_color_hex(C_ACCENT), 0);

    lv_obj_t* spindle_label = lv_label_create(s_spindle_slider);
    lv_label_set_text(spindle_label, "Spindle: 0 RPM");
    lv_obj_set_pos(spindle_label, 10, 10);
    lv_obj_set_style_text_color(spindle_label, lv_color_hex(C_WHITE), 0);
    lv_obj_set_style_text_font(spindle_label, &lv_font_montserrat_12, 0);
}

/**
 * @brief Create control buttons
 */
static void create_control_buttons(lv_obj_t* parent)
{
    int btn_y = 560;
    int btn_w = 60;
    int btn_h = 40;
    int gap = 10;

    /* START button */
    s_btn_start = lv_btn_create(parent);
    lv_obj_set_size(s_btn_start, btn_w, btn_h);
    lv_obj_set_pos(s_btn_start, 10, btn_y);
    lv_obj_set_style_bg_color(s_btn_start, lv_color_hex(C_GREEN), 0);
    lv_obj_set_style_text_color(s_btn_start, lv_color_hex(C_BG_DARK), 0);
    lv_obj_t* start_label = lv_label_create(s_btn_start);
    lv_label_set_text(start_label, "START");
    lv_obj_center(start_label);

    /* PAUSE button */
    s_btn_pause = lv_btn_create(parent);
    lv_obj_set_size(s_btn_pause, btn_w, btn_h);
    lv_obj_set_pos(s_btn_pause, 10 + btn_w + gap, btn_y);
    lv_obj_set_style_bg_color(s_btn_pause, lv_color_hex(C_YELLOW), 0);
    lv_obj_set_style_text_color(s_btn_pause, lv_color_hex(C_BG_DARK), 0);
    lv_obj_t* pause_label = lv_label_create(s_btn_pause);
    lv_label_set_text(pause_label, "PAUSE");
    lv_obj_center(pause_label);

    /* STOP button */
    s_btn_stop = lv_btn_create(parent);
    lv_obj_set_size(s_btn_stop, btn_w, btn_h);
    lv_obj_set_pos(s_btn_stop, 10 + (btn_w + gap) * 2, btn_y);
    lv_obj_set_style_bg_color(s_btn_stop, lv_color_hex(C_RED), 0);
    lv_obj_set_style_text_color(s_btn_stop, lv_color_hex(C_WHITE), 0);
    lv_obj_t* stop_label = lv_label_create(s_btn_stop);
    lv_label_set_text(stop_label, "STOP");
    lv_obj_center(stop_label);

    /* HOME button */
    s_btn_home = lv_btn_create(parent);
    lv_obj_set_size(s_btn_home, btn_w, btn_h);
    lv_obj_set_pos(s_btn_home, 10 + (btn_w + gap) * 3, btn_y);
    lv_obj_set_style_bg_color(s_btn_home, lv_color_hex(C_ACCENT), 0);
    lv_obj_set_style_text_color(s_btn_home, lv_color_hex(C_BG_DARK), 0);
    lv_obj_t* home_label = lv_label_create(s_btn_home);
    lv_label_set_text(home_label, "HOME");
    lv_obj_center(home_label);

    /* ZERO XYZ button */
    s_btn_zero = lv_btn_create(parent);
    lv_obj_set_size(s_btn_zero, btn_w, btn_h);
    lv_obj_set_pos(s_btn_zero, 10 + (btn_w + gap) * 4, btn_y);
    lv_obj_set_style_bg_color(s_btn_zero, lv_color_hex(C_ACCENT), 0);
    lv_obj_set_style_text_color(s_btn_zero, lv_color_hex(C_BG_DARK), 0);
    lv_obj_t* zero_label = lv_label_create(s_btn_zero);
    lv_label_set_text(zero_label, "ZERO");
    lv_obj_center(zero_label);
}

/**
 * @brief Create emergency stop button
 */
static void create_estop_button(lv_obj_t* parent)
{
    int screen_w = lv_obj_get_width(parent);
    int estop_w = 200;
    int estop_h = 60;
    int estop_x = (screen_w - SIDEBAR_WIDTH - estop_w) / 2;
    int estop_y = lv_obj_get_height(parent) - estop_h - 10;

    s_btn_estop = lv_btn_create(parent);
    lv_obj_set_size(s_btn_estop, estop_w, estop_h);
    lv_obj_set_pos(s_btn_estop, estop_x, estop_y);
    lv_obj_set_style_bg_color(s_btn_estop, lv_color_hex(C_RED), 0);
    lv_obj_set_style_text_color(s_btn_estop, lv_color_hex(C_WHITE), 0);
    lv_obj_set_style_radius(s_btn_estop, 12, 0);
    lv_obj_set_style_border_width(s_btn_estop, 3, 0);
    lv_obj_set_style_border_color(s_btn_estop, lv_color_hex(0x880000), 0);

    lv_obj_t* estop_label = lv_label_create(s_btn_estop);
    lv_label_set_text(estop_label, "EMERGENCY STOP");
    lv_obj_set_style_text_font(estop_label, &lv_font_montserrat_20, 0);
    lv_obj_center(estop_label);
}

void scr_home_create(void)
{
    ESP_LOGI(TAG, "Creating home screen");

    /* Create screen */
    s_screen = lv_obj_create(NULL);
    lv_obj_set_size(s_screen, 1024, 600);
    lv_obj_set_style_bg_color(s_screen, lv_color_hex(C_BG_DARK), 0);

    /* Create sidebar (right panel) */
    lv_obj_t* sidebar = lv_obj_create(s_screen);
    lv_obj_set_size(sidebar, SIDEBAR_WIDTH, 600);
    lv_obj_set_pos(sidebar, 1024 - SIDEBAR_WIDTH, 0);
    lv_obj_set_style_bg_color(sidebar, lv_color_hex(C_PANEL), 0);
    lv_obj_set_style_border_width(sidebar, 0, 0);

    /* Create components */
    create_status_bar(s_screen);
    create_preview_area(s_screen);
    create_jog_pad(sidebar);
    create_step_selector(sidebar);
    create_sliders(sidebar);
    create_control_buttons(sidebar);
    create_estop_button(s_screen);

    /* Load screen */
    lv_scr_load(s_screen);

    ESP_LOGI(TAG, "Home screen created");
}

lv_obj_t* scr_home_get_screen(void)
{
    return s_screen;
}

void scr_home_update_status(const char* status, uint16_t buf_usage)
{
    if (s_grbl_status_label) {
        lv_label_set_text(s_grbl_status_label, status);
    }
    if (s_buffer_label) {
        char buf[32];
        snprintf(buf, sizeof(buf), "Buf: %d", buf_usage);
        lv_label_set_text(s_buffer_label, buf);
    }
}

void scr_home_update_position(float x, float y, float z)
{
    if (s_pos_x_label) {
        char buf[32];
        snprintf(buf, sizeof(buf), "X: %.3f", x);
        lv_label_set_text(s_pos_x_label, buf);
    }
    if (s_pos_y_label) {
        char buf[32];
        snprintf(buf, sizeof(buf), "Y: %.3f", y);
        lv_label_set_text(s_pos_y_label, buf);
    }
    if (s_pos_z_label) {
        char buf[32];
        snprintf(buf, sizeof(buf), "Z: %.3f", z);
        lv_label_set_text(s_pos_z_label, buf);
    }
}

void scr_home_update_connection(bool connected)
{
    if (s_conn_label) {
        if (connected) {
            lv_label_set_text(s_conn_label, "Conectado");
            lv_obj_set_style_text_color(s_conn_label, lv_color_hex(C_GREEN), 0);
        } else {
            lv_label_set_text(s_conn_label, "Desconectado");
            lv_obj_set_style_text_color(s_conn_label, lv_color_hex(C_RED), 0);
        }
    }
}
