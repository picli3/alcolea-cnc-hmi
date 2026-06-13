/**
 * @file theme_alcolea.h
 * @brief Alcolea CNC Theme for LVGL
 */

#pragma once

#include "lvgl/lvgl.h"

/* Theme colors */
#define THEME_COLOR_BG_DARK      0x1A1A2E
#define THEME_COLOR_BG_MED       0x16213E
#define THEME_COLOR_BG_LIGHT     0x0F3460
#define THEME_COLOR_ACCENT       0x00D9FF
#define THEME_COLOR_PRIMARY     0x0F3460
#define THEME_COLOR_SECONDARY   0x16213E
#define THEME_COLOR_TEXT         0xFFFFFF
#define THEME_COLOR_TEXT_DIM     0x808080
#define THEME_COLOR_RED          0xFF4444
#define THEME_COLOR_GREEN       0x44FF44
#define THEME_COLOR_YELLOW      0xFFFF44
#define THEME_COLOR_ORANGE      0xFF8800
#define THEME_COLOR_CYAN        0x00D9FF

/* Style names */
typedef enum {
    THEME_STYLE_BG_DARK,
    THEME_STYLE_PANEL,
    THEME_STYLE_BTN_PRIMARY,
    THEME_STYLE_BTN_DANGER,
    THEME_STYLE_BTN_SUCCESS,
    THEME_STYLE_LABEL_TITLE,
    THEME_STYLE_LABEL_NORMAL,
    THEME_STYLE_LABEL_DIM,
    THEME_STYLE_STATUS_BAR,
    THEME_STYLE_COUNT
} theme_style_t;

/**
 * @brief Initialize theme
 */
void theme_alcolea_init(void);

/**
 * @brief Get style by name
 */
lv_style_t* theme_alcolea_get_style(theme_style_t style);
