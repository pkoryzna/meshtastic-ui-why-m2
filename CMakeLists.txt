# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)



# "Trim" the build. Include the minimal set of components, main, and anything it depends on.
idf_build_set_property(MINIMAL_BUILD ON)

# huuuuh? those have to be defined before project?
list(APPEND compile_definitions "ARDUINO_USB_CDC_ON_BOOT=1")
list(APPEND compile_definitions "ARDUINO_USB_MODE=1")
list(APPEND compile_definitions "INPUTDRIVER_TCA8418_KBD_TYPE")
list(APPEND compile_definitions "I2C_SDA=18")
list(APPEND compile_definitions "I2C_SCL=20")

list(APPEND compile_definitions "CORE_DEBUG_LEVEL=ARDUHAL_LOG_LEVEL_VERBOSE")
list(APPEND compile_definitions "LOG_LOCAL_LEVEL=ESP_LOG_VERBOSE")
list(APPEND compile_definitions "CONFIG_ARDUHAL_ESP_LOG")
list(APPEND compile_definitions "CONFIG_ARDUHAL_LOG_COLORS=1")
list(APPEND compile_definitions "USE_ILOG")
list(APPEND compile_definitions "ARDUINO_ARCH_ESP32")

project(meshtastic-ui-why-m2)


idf_build_set_property(CXX_COMPILE_OPTIONS      
      "-w"
      "-Wno-error=format"
      "-Wno-error=overloaded-virtual"
      "-Wno-error=trigraphs"
      "-std=c++14"
      "-Os"

      "-DLV_CONF_INCLUDE_SIMPLE=1"
      "-DLV_COMP_CONF_INCLUDE_SIMPLE=1"
      "-DLV_BUILD_TEST=0"
      "-Isrc"
      "-Isrc/mesh/generated"
      "-fpermissive"
      "-DARCH_ESP32"
      "-DHAS_TFT=1"
      "-DVIEW_320x240"
      "-Ilib"
      "-Ilib/mesh/generated"
      "-Ivariants/why2025-m2"
      "-DUSE_ESP_LCD_WHY2025"
      "-I3rdparty/esp_lcd_st7703/include"
      "-I3rdparty/"
      "-DCONFIG_LITTLEFS_PAGE_SIZE=256"
      "-DBOARD_HAS_PSRAM"
      "-DARDUINO_USB_CDC_ON_BOOT=1"
      "-DARDUINO_USB_MODE=1"
      "-DARDUINO_RUNNING_CORE=1"
      "-DARDUINO_EVENT_RUNNING_CORE=1"
      "-DLV_LVGL_H_INCLUDE_SIMPLE"
 APPEND)
