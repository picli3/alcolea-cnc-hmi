/**
 * @file wgt_jog_pad.c
 * @brief Jog pad widget implementation
 */

#include "wgt_jog_pad.h"
#include "esp_log.h"

static const char* TAG = "wgt_jog_pad";

lv_obj_t* wgt_jog_pad_create(lv_obj_t* parent, jog_callback_t callback, void* user_data)
{
    if (!parent) return NULL;
    /* TODO: Implement jog pad widget */
    return parent;
}

void wgt_jog_pad_set_steps(lv_obj_t* widget, const float* steps, size_t count)
{
    if (!widget) return;
    /* TODO: Implement step selector */
}
