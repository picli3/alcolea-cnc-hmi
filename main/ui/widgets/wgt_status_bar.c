/**
 * @file wgt_status_bar.c
 * @brief Status bar widget implementation
 */

#include "wgt_status_bar.h"
#include "esp_log.h"

static const char* TAG = "wgt_status_bar";

void wgt_status_bar_update(lv_obj_t* widget, const char* grbl_status,
                          uint16_t planner_buf, uint16_t rx_buf)
{
    if (!widget) return;
    /* TODO: Implement status bar update */
}
