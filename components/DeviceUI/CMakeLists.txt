cmake_minimum_required(VERSION 3.15)


set(GENERATED_VIEW ${VIEW} "ui_320x240")

message(STATUS "Using C++${CMAKE_CXX_STANDARD}")
message(STATUS "Using Generated View: " ${GENERATED_VIEW})

# add_compile_definitions(VIEW_320x240)

# include(FetchContent)
# include(Portduino)
# include(LovyanGFX)
# include(LVGL)
# include(protobuf)
# include(nanopb)
# include(Doctest)

file(GLOB_RECURSE sources      source/* generated/* locale/* generated/${GENERATED_VIEW}/* source/util/* drivers/*)
file(GLOB_RECURSE sources_test tests/*.cpp)

idf_component_register(
    SRCS ${sources}
    INCLUDE_DIRS 
        "."
        include/
        src/
        locale/
        # portduino/
        generated/${GENERATED_VIEW}/
        ${GENERATED_FILES_DIR}/
        ${CMAKE_CURRENT_BINARY_DIR}/
        drivers/
    REQUIRES
        MeshtasticProtobuf
    PRIV_REQUIRES
        lvgl
        nanopb
        esp_driver_uart
        arduino-esp32
        SdFat
        esp_lcd_st7703
)

target_compile_options(${COMPONENT_LIB} PRIVATE
      "-w"
      "-Wno-error=format"
      "-Wno-error=overloaded-virtual"
      "-Wno-error=trigraphs"
      "-std=c++14"
      "-Os"
      "-DCORE_DEBUG_LEVEL=ARDUHAL_LOG_LEVEL_VERBOSE"
      "-DLOG_LOCAL_LEVEL=ESP_LOG_VERBOSE"
      "-DCONFIG_ARDUHAL_ESP_LOG"
      "-DCONFIG_ARDUHAL_LOG_COLORS=1"
      "-DLV_CONF_INCLUDE_SIMPLE=1"
      "-DLV_COMP_CONF_INCLUDE_SIMPLE=1"
      "-DLV_BUILD_TEST=0"
      "-Isrc"
      "-Isrc/mesh/generated"
      "-fpermissive"
      "-DARCH_ESP32"
      "-DHAS_TFT=1"
      "-DVIEW_320x240"
      "-DARDUINO_ARCH_ESP32"
      "-Ilib"
      "-Ilib/mesh/generated"
      "-Ivariants/why2025-m2"
      "-DSERIAL_RX=15"
      "-DSERIAL_TX=16"
      "-DUSE_SERIAL1"
      "-DUSE_ILOG"
      "-I3rdparty/esp_lcd_st7703/include"
      "-I3rdparty/"
      "-DCONFIG_LITTLEFS_PAGE_SIZE=256"
      "-DBOARD_HAS_PSRAM"
      "-DARDUINO_USB_CDC_ON_BOOT=1"
      "-DARDUINO_USB_MODE=1"
      "-DARDUINO_RUNNING_CORE=1"
      "-DARDUINO_EVENT_RUNNING_CORE=1"
      "-DLV_LVGL_H_INCLUDE_SIMPLE"
)