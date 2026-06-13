/**
 * @file theme_alcolea.c
 * @brief Alcolea CNC Theme implementation
 */

#include "theme_alcolea.h"
#include "esp_log.h"

static const char* TAG = "theme";

static lv_style_t s_styles[THEME_STYLE_COUNT];
static bool s_initialized = false;

/**
 * @brief Create a style with common properties
 */
static void make_style(theme_style_t style_id, lv_style_prop_t prop, lv_style_val_t value)
{
    lv_style_init(&s_styles[style_id]);
    lv_style_set_bg_color(&s_styles[style_id], lv_color_hex(THEME_COLOR_BG_DARK));
    lv_style_set_text_color(&s_styles[style_id], lv_color_hex(THEME_COLOR_TEXT));
    lv_style_set_radius(&s_styles[style_id], 8);

    if (prop != 0) {
        lv_style_set_prop(&s_styles[style_id], prop, value);
    }
}

void theme_alcolea_init(void)
{
    if (s_initialized) {
        return;
    }

    ESP_LOGI(TAG, "Initializing Alcolea theme");

    /* Background dark */
    make_style(THEME_STYLE_BG_DARK, LV_STYLE_BG_COLOR,
                lv_style_val_make(LV_STYLE_BG_COLOR, lv_color_hex(THEME_COLOR_BG_DARK)));

    /* Panel style */
    make_style(THEME_STYLE_PANEL, LV_STYLE_BG_COLOR,
                lv_style_val_make(LV_STYLE_BG_COLOR, lv_color_hex(THEME_COLOR_BG_MED)));
    lv_style_set_border_width(&s_styles[THEME_STYLE_PANEL], 2);
    lv_style_set_border_color(&s_styles[THEME_STYLE_PANEL], lv_color_hex(THEME_COLOR_ACCENT));
    lv_style_set_pad_all(&s_styles[THEME_STYLE_PANEL], 10);

    /* Button primary */
    make_style(THEME_STYLE_BTN_PRIMARY, LV_STYLE_BG_COLOR,
                lv_style_val_make(LV_STYLE_BG_COLOR, lv_color_hex(THEME_COLOR_ACCENT)));
    lv_style_set_text_color(&s_styles[THEME_STYLE_BTN_PRIMARY], lv_color_hex(THEME_COLOR_TEXT));
    lv_style_set_radius(&s_styles[THEME_STYLE_BTN_PRIMARY], 12);
    lv_style_set_pad_hor(&s_styles[THEME_STYLE_BTN_PRIMARY], 20);
    lv_style_set_pad_ver(&s_styles[THEME_STYLE_BTN_PRIMARY], 10);

    /* Button danger (red) */
    make_style(THEME_STYLE_BTN_DANGER, LV_STYLE_BG_COLOR,
                lv_style_val_make(LV_STYLE_BG_COLOR, lv_color_hex(THEME_COLOR_RED)));
    lv_style_set_text_color(&s_styles[THEME_STYLE_BTN_DANGER], lv_color_hex(THEME_COLOR_TEXT));
    lv_style_set_radius(&s_styles[THEME_STYLE_BTN_DANGER], 12);

    /* Button success (green) */
    make_style(THEME_STYLE_BTN_SUCCESS, LV_STYLE_BG_COLOR,
                lv_style_val_make(LV_STYLE_BG_COLOR, lv_color_hex(THEME_COLOR_GREEN)));
    lv_style_set_text_color(&s_styles[THEME_STYLE_BTN_SUCCESS], lv_color_hex(THEME_COLOR_BG_DARK));
    lv_style_set_radius(&s_styles[THEME_STYLE_BTN_SUCCESS], 12);

    /* Label title */
    make_style(THEME_STYLE_LABEL_TITLE, LV_STYLE_TEXT_COLOR,
                lv_style_val_make(LV_STYLE_TEXT_COLOR, lv_color_hex(THEME_COLOR_ACCENT)));
    lv_style_set_text_font(&s_styles[THEME_STYLE_LABEL_TITLE], &lv_font_montserrat_24);

    /* Label normal */
    make_style(THEME_STYLE_LABEL_NORMAL, LV_STYLE_TEXT_COLOR,
                lv_style_val_make(LV_STYLE_TEXT_COLOR, lv_color_hex(THEME_COLOR_TEXT)));
    lv_style_set_text_font(&s_styles[THEME_STYLE_LABEL_NORMAL], &lv_font_montserrat_16);

    /* Label dim */
    make_style(THEME_STYLE_LABEL_DIM, LV_STYLE_TEXT_COLOR,
                lv_style_val_make(LV_STYLE_TEXT_COLOR, lv_color_hex(THEME_COLOR_TEXT_DIM)));
    lv_style_set_text_font(&s_styles[THEME_STYLE_LABEL_DIM], &lv_font_montserrat_14);

    /* Status bar */
    make_style(THEME_STYLE_STATUS_BAR, LV_STYLE_BG_COLOR,
                lv_style_val_make(LV_STYLE_BG_COLOR, lv_color_hex(THEME_COLOR_BG_MED)));
    lv_style_set_border_width(&s_styles[THEME_STYLE_STATUS_BAR], 0);
    lv_style_set_pad_all(&s_styles[THEME_STYLE_STATUS_BAR], 8);

    s_initialized = true;
    ESP_LOGI(TAG, "Theme initialized");
}

lv_style_t* theme_alcolea_get_style(theme_style_t style)
{
    if (style >= THEME_STYLE_COUNT) {
        return NULL;
    }
    return &s_styles[style];
}
