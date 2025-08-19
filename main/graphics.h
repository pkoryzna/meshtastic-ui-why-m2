#include "freertos/semphr.h"
#include "lvgl.h"

#define LVGL_DRAW_BUF_LINES    200 // number of display lines in each draw buffer
#define LVGL_TICK_PERIOD_MS    2
#define LVGL_TASK_MAX_DELAY_MS 500
#define LVGL_TASK_MIN_DELAY_MS 1
#define LVGL_TASK_STACK_SIZE   (4 * 1024)
#define LVGL_TASK_PRIORITY     2

extern SemaphoreHandle_t lvgl_api_mux;
bool lvgl_lock(int timeout_ms);
void lvgl_unlock(void);
void bsp_set_lcd_backlight(uint32_t level);
lv_display_t* lcd_init();