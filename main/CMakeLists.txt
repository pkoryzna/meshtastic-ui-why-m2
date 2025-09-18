idf_component_register(SRCS
        "main.cpp"
        "c6_fw_flasher.cpp"
    PRIV_REQUIRES
        esp_driver_uart
        esp_driver_usb_serial_jtag
        arduino-esp32
        ArduinoThread
        DeviceUI
        SdFat
        nanopb
        MeshtasticProtobuf
        spi_flash
        lvgl
        esp_psram
        esp_timer
        esp_lcd
        esp_lcd_st7703
        esp_hw_support
        esp_mm

    INCLUDE_DIRS
        "."
    EMBED_TXTFILES
        "meshtastic_fw_version"
    )

file(READ "meshtastic_fw_version" MESHTASTIC_VER)

add_custom_target(firmware-why2025-carrier ALL
        COMMAND ${COMPONENT_DIR}/get_firmware.sh ${MESHTASTIC_VER}
)

add_dependencies(${COMPONENT_LIB} firmware-why2025-carrier)

target_add_binary_data(${COMPONENT_LIB} ${CMAKE_CURRENT_BINARY_DIR}/c6-boot_app0.bin BINARY)
target_add_binary_data(${COMPONENT_LIB} ${CMAKE_CURRENT_BINARY_DIR}/c6-boot_app0.bin.md5 TEXT)
target_add_binary_data(${COMPONENT_LIB} ${CMAKE_CURRENT_BINARY_DIR}/c6-bootloader.bin BINARY)
target_add_binary_data(${COMPONENT_LIB} ${CMAKE_CURRENT_BINARY_DIR}/c6-bootloader.bin.md5 TEXT)
target_add_binary_data(${COMPONENT_LIB} ${CMAKE_CURRENT_BINARY_DIR}/c6-partitions.bin BINARY)
target_add_binary_data(${COMPONENT_LIB} ${CMAKE_CURRENT_BINARY_DIR}/c6-partitions.bin.md5 TEXT)
target_add_binary_data(${COMPONENT_LIB} ${CMAKE_CURRENT_BINARY_DIR}/c6-firmware.bin BINARY)
target_add_binary_data(${COMPONENT_LIB} ${CMAKE_CURRENT_BINARY_DIR}/c6-firmware.bin.md5 TEXT)
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
      "-DARDUINO_ARCH_ESP32"
      "-DHAS_TFT=1"
      "-DVIEW_320x240"
      "-Ilib"
      "-Ilib/mesh/generated"
      "-Ivariants/why2025-m2"
      "-DUSE_ILOG"
      "-I3rdparty/esp_lcd_st7703/include"
      "-I3rdparty/"
      "-DCONFIG_LITTLEFS_PAGE_SIZE=256"
      "-DBOARD_HAS_PSRAM"
      "-DARDUINO_USB_CDC_ON_BOOT=1"
      "-DARDUINO_USB_MODE=1"
      "-DARDUINO_RUNNING_CORE=1"
      "-DARDUINO_EVENT_RUNNING_CORE=0"
      "-DLV_LVGL_H_INCLUDE_SIMPLE"
)
