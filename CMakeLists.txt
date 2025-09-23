# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)



# "Trim" the build. Include the minimal set of components, main, and anything it depends on.
idf_build_set_property(MINIMAL_BUILD ON)

# those have to be defined before project or else they won't get applied to components :(
list(APPEND compile_definitions "ARDUINO_USB_CDC_ON_BOOT=1")
list(APPEND compile_definitions "ARDUINO_USB_MODE=1")
list(APPEND compile_definitions "I2C_SDA=18")
list(APPEND compile_definitions "I2C_SCL=20")

list(APPEND compile_definitions "CORE_DEBUG_LEVEL=ARDUHAL_LOG_LEVEL_VERBOSE")
list(APPEND compile_definitions "LOG_LOCAL_LEVEL=ESP_LOG_VERBOSE")
list(APPEND compile_definitions "CONFIG_ARDUHAL_ESP_LOG")
list(APPEND compile_definitions "CONFIG_ARDUHAL_LOG_COLORS=1")
list(APPEND compile_definitions "USE_ILOG")
list(APPEND compile_definitions "ARDUINO_ARCH_ESP32")
# LVGL cofig 
list(APPEND compile_definitions "LV_CONF_INCLUDE_SIMPLE")
list(APPEND compile_definitions "BOARD_HAS_PSRAM")
list(APPEND compile_definitions "RAM_SIZE=6144") # highest seen on a real device
include_directories("components/DeviceUI/include")

list(APPEND compile_definitions "WHY_BADGE")

# Define WHY_BADGE_CUSTOM_C6_FW if you want to use custom firmware on C6 chip
# this disables the built-in serial flasher completely
# so that your custom FW doesn't get erased
#list(APPEND compile_definitions "WHY_BADGE_CUSTOM_C6_FW")

project(meshtastic-ui-why-m2)


idf_build_set_property(CXX_COMPILE_OPTIONS      
      "-w"
      "-Wno-error=format"
      "-Wno-error=overloaded-virtual"
      "-Wno-error=trigraphs"
      "-std=c++14"
      "-Os"
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
      "-I3rdparty/esp_lcd_st7703/include"
      "-I3rdparty/"
      "-DCONFIG_LITTLEFS_PAGE_SIZE=256"
      "-DARDUINO_USB_CDC_ON_BOOT=1"
      "-DARDUINO_USB_MODE=1"
      "-DARDUINO_RUNNING_CORE=1"
      "-DARDUINO_EVENT_RUNNING_CORE=1"
      "-DLV_LVGL_H_INCLUDE_SIMPLE"
 APPEND)
