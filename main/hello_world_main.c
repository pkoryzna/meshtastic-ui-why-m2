/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "esp_err.h"
#include "esp_log.h"
#include "lvgl.h"
#include "graphics.h"
static const char *TAG = "MAIN";

void app_main(void)
{
    lv_display_t* display = lcd_init();
    if (lvgl_lock(-1)) {
        /*Change the active screen's background color*/
        lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0x003a57), LV_PART_MAIN);

        /*Create a white label, set its text and align it to the center*/
        lv_obj_t * label = lv_label_create(lv_screen_active());
        lv_label_set_text(label, "Hello world");
        lv_obj_set_style_text_color(lv_screen_active(), lv_color_hex(0xffffff), LV_PART_MAIN);
        lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
        lvgl_unlock();
    }
}
