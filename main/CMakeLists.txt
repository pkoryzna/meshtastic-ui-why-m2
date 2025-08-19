idf_component_register(SRCS "hello_world_main.c" "graphics.c"
                       PRIV_REQUIRES 
                        spi_flash
                        lvgl
                        esp_psram
                        esp_timer
                        esp_lcd
                        esp_lcd_st7703
                        esp_hw_support
                        esp_mm
                       INCLUDE_DIRS
                       ".")
