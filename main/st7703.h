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

#define FRAMEBUFFER_MAX_W       720
#define FRAMEBUFFER_MAX_H       720
#define FRAMEBUFFER_MAX_REFRESH 60
#define FRAMEBUFFER_BPP         2 // 16 bits per pixel

#define DISPLAY_FRAMEBUFFERS 2

#define ST7703_720_720_PANEL_60HZ_DPI_CONFIG()                                                                         \
    {.dpi_clk_src        = MIPI_DSI_DPI_CLK_SRC_DEFAULT,                                                               \
     .dpi_clock_freq_mhz = 47,                                                                                         \
     .virtual_channel    = 0,                                                                                          \
     .pixel_format       = (FRAMEBUFFER_BPP == 2 ? LCD_COLOR_PIXEL_FORMAT_RGB565 : LCD_COLOR_PIXEL_FORMAT_RGB888),     \
     .num_fbs            = DISPLAY_FRAMEBUFFERS,                                                                       \
     .video_timing =                                                                                                   \
         {                                                                                                             \
             .h_size            = 720,                                                                                 \
             .v_size            = 720,                                                                                 \
             .hsync_back_porch  = 120,                                                                                 \
             .hsync_pulse_width = 60,                                                                                  \
             .hsync_front_porch = 106,                                                                                 \
             .vsync_back_porch  = 20,                                                                                  \
             .vsync_pulse_width = 4,                                                                                   \
             .vsync_front_porch = 20,                                                                                  \
         },                                                                                                            \
     .flags.use_dma2d  = true,                                                                                         \
     .flags.disable_lp = false}


#define CUSTOM_INIT_CMDS()                                                                                             \
    {                                                                                                                  \
        {0xB9, (uint8_t[]){0xF1, 0x12, 0x83}, 3, 0},                                                                   \
        {0xB1, (uint8_t[]){0x00, 0x00, 0x00, 0xDA, 0x80}, 5, 0},                                                       \
        {0xB2, (uint8_t[]){0x3C, 0x02, 0x30}, 3, 0},                                                                   \
        {0xB3, (uint8_t[]){0x10, 0x10, 0x28, 0x28, 0x03, 0xFF, 0x00, 0x00, 0x00, 0x00}, 10, 0},                        \
        {0xB4, (uint8_t[]){0x80}, 1, 0},                                                                               \
        {0xB5, (uint8_t[]){0x0A, 0x0A}, 2, 0},                                                                         \
        {0xB6, (uint8_t[]){0x97, 0x97}, 2, 0},                                                                         \
        {0xB8, (uint8_t[]){0x26, 0x22, 0xF0, 0x63}, 4, 0},                                                             \
        {0xBA,                                                                                                         \
         (uint8_t[]){0x31, 0x81, 0x05, 0xF9, 0x0E, 0x0E, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,               \
                     0x44, 0x25, 0x00, 0x90, 0x0A, 0x00, 0x00, 0x01, 0x4F, 0x01, 0x00, 0x00, 0x37},                    \
         27,                                                                                                           \
         0},                                                                                                           \
        {0xBC, (uint8_t[]){0x47}, 1, 0},                                                                               \
        {0xBF, (uint8_t[]){0x02, 0x11, 0x00}, 3, 0},                                                                   \
        {0xC0, (uint8_t[]){0x73, 0x73, 0x50, 0x50, 0x00, 0x00, 0x12, 0x70, 0x00}, 9, 0},                               \
        {0xC1, (uint8_t[]){0x43, 0x00, 0x32, 0x32, 0x77, 0xC1, 0xFF, 0xFF, 0xCC, 0xCC, 0x77, 0x77}, 12, 0},            \
        {0xC6, (uint8_t[]){0x82, 0x00, 0xBF, 0xFF, 0x00, 0xFF}, 6, 0},                                                 \
        {0xC7, (uint8_t[]){0xB8, 0x00, 0x0A, 0x00, 0x00, 0x00}, 6, 0},                                                 \
        {0xC8, (uint8_t[]){0x10, 0x40, 0x1E, 0x02}, 4, 0},                                                             \
        {0xCC, (uint8_t[]){0x0B}, 1, 0},                                                                               \
        {0xE0,                                                                                                         \
         (uint8_t[]){0x00, 0x0B, 0x10, 0x2C, 0x3D, 0x3F, 0x42, 0x3A, 0x07, 0x0D, 0x0F, 0x13,                           \
                     0x15, 0x13, 0x14, 0x0F, 0x16, 0x00, 0x0B, 0x10, 0x2C, 0x3D, 0x3F, 0x42,                           \
                     0x3A, 0x07, 0x0D, 0x0F, 0x13, 0x15, 0x13, 0x14, 0x0F, 0x16},                                      \
         34,                                                                                                           \
         0},                                                                                                           \
        {0xE3,                                                                                                         \
         (uint8_t[]){0x07, 0x07, 0x0B, 0x0B, 0x0B, 0x0B, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xC0, 0x10},              \
         14,                                                                                                           \
         0},                                                                                                           \
        {0xE9,                                                                                                         \
         (uint8_t[]){0xC8, 0x10, 0x0A, 0x00, 0x00, 0x80, 0x81, 0x12, 0x31, 0x23, 0x4F, 0x86, 0xA0, 0x00, 0x47, 0x08,   \
                     0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x98, 0x02, 0x8B, 0xAF,   \
                     0x46, 0x02, 0x88, 0x88, 0x88, 0x88, 0x88, 0x98, 0x13, 0x8B, 0xAF, 0x57, 0x13, 0x88, 0x88, 0x88,   \
                     0x88, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},        \
         63,                                                                                                           \
         0},                                                                                                           \
        {0xEA,                                                                                                         \
         (uint8_t[]){0x97, 0x0C, 0x09, 0x09, 0x09, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9F, 0x31, 0x8B, 0xA8,   \
                     0x31, 0x75, 0x88, 0x88, 0x88, 0x88, 0x88, 0x9F, 0x20, 0x8B, 0xA8, 0x20, 0x64, 0x88, 0x88, 0x88,   \
                     0x88, 0x88, 0x23, 0x00, 0x00, 0x02, 0x62, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   \
                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x80, 0x81, 0x00, 0x00, 0x00, 0x00},                    \
         61,                                                                                                           \
         0},                                                                                                           \
        {0xEF, (uint8_t[]){0xFF, 0xFF, 0x01}, 3, 0},                                                                   \
        {0x11, (uint8_t[]){}, 0, 250},                                                                                 \
        {0x29, (uint8_t[]){}, 0, 50},                                                                                  \
    }


#define LCD_IO_RST             (GPIO_NUM_17) // -1 if not used
#define PIN_NUM_BK_LIGHT       (-1)          // -1 if not used
#define LCD_BK_LIGHT_ON_LEVEL  (1)
#define LCD_BK_LIGHT_OFF_LEVEL !LCD_BK_LIGHT_ON_LEVEL

#define MIPI_DSI_PHY_PWR_LDO_CHAN       (3) // LDO_VO3 is connected to VDD_MIPI_DPHY
#define MIPI_DSI_PHY_PWR_LDO_VOLTAGE_MV (2500)
#define LCD_MIPI_DSI_BUS_ID             (0)
#define LCD_MIPI_DSI_LANE_NUM           (2)    // 2 data lanes
#define LCD_MIPI_DSI_LANE_BITRATE_MBPS  (1000) // 1Gbps

