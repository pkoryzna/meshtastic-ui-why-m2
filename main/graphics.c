// Based on examples from Espressif under license:
/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */
// Based on BadgeVMS source code under license below:
/* This file is part of BadgeVMS
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */



#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_timer.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_mipi_dsi.h"
#include "esp_lcd_st7703.h"
#include "esp_ldo_regulator.h"
#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_log.h"
#include "lvgl.h"
#include "graphics.h"
#include "st7703.h"

static st7703_lcd_init_cmd_t const custom_init[] = CUSTOM_INIT_CMDS();

static const char* TAG = "GRPH";

SemaphoreHandle_t lvgl_api_mux = NULL;

void lvgl_flush_cb(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map)
{
    esp_lcd_panel_handle_t panel_handle = (esp_lcd_panel_handle_t)lv_display_get_user_data(disp);
    int offsetx1 = area->x1;
    int offsetx2 = area->x2;
    int offsety1 = area->y1;
    int offsety2 = area->y2;
    // pass the draw buffer to the driver
    // TODO: rotate the screen lol
    esp_lcd_panel_draw_bitmap(panel_handle, offsetx1, offsety1, offsetx2 + 1, offsety2 + 1, px_map);
}

void increase_lvgl_tick(void *arg)
{
    /* Tell LVGL how many milliseconds has elapsed */
    lv_tick_inc(LVGL_TICK_PERIOD_MS);
}

bool lvgl_lock(int timeout_ms)
{
    // Convert timeout in milliseconds to FreeRTOS ticks
    // If `timeout_ms` is set to -1, the program will block until the condition is met
    const TickType_t timeout_ticks = (timeout_ms == -1) ? portMAX_DELAY : pdMS_TO_TICKS(timeout_ms);
    return xSemaphoreTakeRecursive(lvgl_api_mux, timeout_ticks) == pdTRUE;
}

void lvgl_unlock(void)
{
    xSemaphoreGiveRecursive(lvgl_api_mux);
}

void lvgl_port_task(void *arg)
{
    ESP_LOGI(TAG, "Starting LVGL task");
    uint32_t task_delay_ms = LVGL_TASK_MAX_DELAY_MS;
    while (1) {
        // Lock the mutex due to the LVGL APIs are not thread-safe
        if (lvgl_lock(-1)) {
            task_delay_ms = lv_timer_handler();
            // Release the mutex
            lvgl_unlock();
        }
        if (task_delay_ms > LVGL_TASK_MAX_DELAY_MS) {
            task_delay_ms = LVGL_TASK_MAX_DELAY_MS;
        } else if (task_delay_ms < LVGL_TASK_MIN_DELAY_MS) {
            task_delay_ms = LVGL_TASK_MIN_DELAY_MS;
        }
        vTaskDelay(pdMS_TO_TICKS(task_delay_ms));
    }
}

bool notify_lvgl_flush_ready(esp_lcd_panel_handle_t panel, esp_lcd_dpi_panel_event_data_t *edata, void *user_ctx)
{
    lv_display_t *disp = (lv_display_t *)user_ctx;
    lv_display_flush_ready(disp);
    return false;
}

static esp_err_t enable_dsi_phy_power(void) {
    ESP_LOGI(TAG, "Powering on MIPI DSI PHY");
    // Turn on the power for MIPI DSI PHY, so it can go from "No Power" state to "Shutdown" state
    static esp_ldo_channel_handle_t phy_pwr_chan = NULL;
    esp_ldo_channel_config_t        ldo_cfg      = {
                    .chan_id    = MIPI_DSI_PHY_PWR_LDO_CHAN,
                    .voltage_mv = MIPI_DSI_PHY_PWR_LDO_VOLTAGE_MV,
    };
    return esp_ldo_acquire_channel(&ldo_cfg, &phy_pwr_chan);
}


lv_display_t* lcd_init()
{
    ESP_ERROR_CHECK(enable_dsi_phy_power());

    ESP_LOGI(TAG, "Creating MIPI DSI bus");
    esp_lcd_dsi_bus_handle_t mipi_dsi_bus = NULL;
    esp_lcd_dsi_bus_config_t bus_config   = {
          .bus_id             = LCD_MIPI_DSI_BUS_ID,
          .num_data_lanes     = LCD_MIPI_DSI_LANE_NUM,
          .phy_clk_src        = MIPI_DSI_PHY_CLK_SRC_DEFAULT,
          .lane_bit_rate_mbps = LCD_MIPI_DSI_LANE_BITRATE_MBPS,
    };
    ESP_ERROR_CHECK(esp_lcd_new_dsi_bus(&bus_config, &mipi_dsi_bus));

    ESP_LOGI(TAG, "Install panel IO");
    esp_lcd_panel_io_handle_t io         = NULL;
    // we use DBI interface to send LCD commands and parameters
    esp_lcd_dbi_io_config_t  dbi_config = {
          .virtual_channel = 0,
          .lcd_cmd_bits    = 8, // according to the LCD spec
          .lcd_param_bits  = 8, // according to the LCD spec
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_dbi(mipi_dsi_bus, &dbi_config, &io));
   
    ESP_LOGI(TAG, "Seting up ST7703 LCD device");
    esp_lcd_dpi_panel_config_t dpi_config = ST7703_720_720_PANEL_60HZ_DPI_CONFIG();

    st7703_vendor_config_t vendor_config = {
        .mipi_config =
            {
                .dsi_bus    = mipi_dsi_bus,
                .dpi_config = &dpi_config,
            },
        .init_cmds      = custom_init,
        .init_cmds_size = sizeof(custom_init) / sizeof(st7703_lcd_init_cmd_t),
        .init_in_command_mode = true, // this made it work. badgevms firmware may be doing something different
    };

    esp_lcd_panel_dev_config_t lcd_dev_config = {
        .bits_per_pixel          = (FRAMEBUFFER_BPP * 8),
        .rgb_ele_order           = LCD_RGB_ELEMENT_ORDER_RGB,
        .reset_gpio_num          = LCD_IO_RST,
        .vendor_config           = &vendor_config,
        .flags.reset_active_high = 1,
    };

    esp_lcd_panel_handle_t panel_handle = NULL;
    ESP_ERROR_CHECK(esp_lcd_new_panel_st7703(io, &lcd_dev_config, &panel_handle));
    // Not supported by this panel!
    // ESP_ERROR_CHECK(esp_lcd_panel_swap_xy(panel_handle, true));
    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));

    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));

    ESP_LOGI(TAG, "Initializing LVGL and display");
    lv_init();
    lv_display_t *display = lv_display_create(FRAMEBUFFER_MAX_W, FRAMEBUFFER_MAX_H);
    lv_display_set_color_format(display, LV_COLOR_FORMAT_RGB565);

    // store the MIPI panel handle as our LVGL display's user data, to be passed into callbacks later
    lv_display_set_user_data(display, panel_handle);

    // create draw buffers
    void *buf1 = NULL;
    void *buf2 = NULL;

    size_t draw_buffer_sz = FRAMEBUFFER_MAX_W * LVGL_DRAW_BUF_LINES * FRAMEBUFFER_BPP;

    buf1 = heap_caps_malloc(draw_buffer_sz, MALLOC_CAP_DEFAULT);
    assert(buf1);
    buf2 = heap_caps_malloc(draw_buffer_sz, MALLOC_CAP_DEFAULT);
    assert(buf2);

    // initialize LVGL draw buffers
    lv_display_set_buffers(display, buf1, buf2, draw_buffer_sz, LV_DISPLAY_RENDER_MODE_PARTIAL);
    // set the callback to copy the updated buffer to the display
    lv_display_set_flush_cb(display, lvgl_flush_cb);

    esp_lcd_dpi_panel_event_callbacks_t cbs = {
        // notify LVGL when our data has been copied to LCD driver's fb
        .on_color_trans_done = notify_lvgl_flush_ready,
        // don't need to wait for the full vertical refresh, i think?
        // .on_refresh_done = notify_lvgl_flush_ready,
   };
    ESP_ERROR_CHECK(esp_lcd_dpi_panel_register_event_callbacks(panel_handle, &cbs, display));

    ESP_LOGI(TAG, "Create esp_timer for LVGL tick");
    const esp_timer_create_args_t lvgl_tick_timer_args = {
        .callback = &increase_lvgl_tick,
        .name = "lvgl_tick"
    };
    esp_timer_handle_t lvgl_tick_timer = NULL;
    ESP_ERROR_CHECK(esp_timer_create(&lvgl_tick_timer_args, &lvgl_tick_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(lvgl_tick_timer, LVGL_TICK_PERIOD_MS * 1000));

    // Create mutex for locking LVGL API calls
    lvgl_api_mux = xSemaphoreCreateRecursiveMutex();
    assert(lvgl_api_mux);

    ESP_LOGI(TAG, "Create LVGL task");
    xTaskCreate(lvgl_port_task, "LVGL", LVGL_TASK_STACK_SIZE, NULL, LVGL_TASK_PRIORITY, NULL);

    return display;
}