/**
 * @file wgt_axis_pos.c
 * @brief Axis position widget implementation
 */

#include "wgt_axis_pos.h"
#include "../styles/theme_alcolea.h"
#include "esp_log.h"

static const char* TAG = "wgt_axis_pos";

#define C_ACCENT    0x00D9FF
#define C_WHITE     0xFFFFFF
#define C_DIM       0x808080

void wgt_axis_pos_update(lv_obj_t* widget, float x, float y, float z, bool is_work)
{
    if (!widget) return;

    /* Find labels within widget */
    lv_obj_t* child = lv_obj_get_child(widget, 0);
    if (child) {
        char buf[64];
        snprintf(buf, sizeof(buf), "X:%.3f Y:%.3f Z:%.3f", x, y, z);
        lv_label_set_text(child, buf);
    }
}
