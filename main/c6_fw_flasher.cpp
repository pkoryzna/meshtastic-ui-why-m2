#include <esp_loader.h>
#include <esp32_port.h>
#include <esp_err.h>
#include <driver/uart.h>
#include <driver/gpio.h>
#include <esp_log.h>

#include "esp32-hal.h"

esp_loader_error_t err;

extern const uint8_t c6_firmware_bin_start[] asm("_binary_c6_firmware_bin_start");
extern const uint8_t c6_firmware_bin_end[] asm("_binary_c6_firmware_bin_end");
extern const uint8_t c6_firmware_bin_md5[] asm("_binary_c6_firmware_bin_md5_start");
constexpr uint32_t c6_firmware_offset = 0x10000;

extern const uint8_t c6_bootloader_bin_start[] asm("_binary_c6_bootloader_bin_start");
extern const uint8_t c6_bootloader_bin_end[] asm("_binary_c6_bootloader_bin_end");
extern const uint8_t c6_bootloader_bin_md5[] asm("_binary_c6_bootloader_bin_md5_start");
constexpr uint32_t c6_bootloader_offset = 0;

extern const uint8_t c6_boot_app0_bin_start[] asm("_binary_c6_boot_app0_bin_start");
extern const uint8_t c6_boot_app0_bin_end[] asm("_binary_c6_boot_app0_bin_end");
extern const uint8_t c6_boot_app0_bin_md5[] asm("_binary_c6_boot_app0_bin_md5_start");
constexpr uint32_t c6_boot_app0_offset = 57344;

extern const uint8_t c6_partitions_bin_start[] asm("_binary_c6_partitions_bin_start");
extern const uint8_t c6_partitions_bin_end[] asm("_binary_c6_partitions_bin_end");
extern const uint8_t c6_partitions_bin_md5[] asm("_binary_c6_partitions_bin_md5_start");
constexpr uint32_t c6_partitions_bin_offset = 32768;
const char* TAG = "c6_fw_flasher";

constexpr uint32_t DEFAULT_BAUD_RATE = 115200;
constexpr uint32_t HIGH_BAUD_RATE = DEFAULT_BAUD_RATE;

constexpr uint32_t after_reboot_delay_ms = 200;

esp_loader_error_t init_connection_for_flashing()
{
    ESP_LOGI(TAG, "Initializing flashing on %s","UART_NUM_1");
    loader_esp32_config_t loader_config = {
        .baud_rate = DEFAULT_BAUD_RATE,
        .uart_port = UART_NUM_1,
        .uart_rx_pin = GPIO_NUM_15,
        .uart_tx_pin = GPIO_NUM_16,
        .reset_trigger_pin = GPIO_NUM_12,
        .gpio0_trigger_pin = GPIO_NUM_13,
    };

    err = loader_port_esp32_init(&loader_config);
    if (err != ESP_LOADER_SUCCESS)
    {
        ESP_LOGE(TAG, "loader_port_esp32_init failed with error %s", esp_err_to_name(err));
        return err;
    }

    esp_loader_connect_args_t esp_loader_connect_args = ESP_LOADER_CONNECT_DEFAULT();
    err = esp_loader_connect_with_stub(&esp_loader_connect_args);
    if (err != ESP_LOADER_SUCCESS)
    {
        ESP_LOGE(TAG, "esp_loader_connect_with_stub failed with error %s", esp_err_to_name(err));
        return err;
    }

    ESP_LOGI(TAG, "Changing baud rate from %d to %d", DEFAULT_BAUD_RATE, HIGH_BAUD_RATE);
    err = esp_loader_change_transmission_rate_stub(DEFAULT_BAUD_RATE, HIGH_BAUD_RATE);
    if (err != ESP_LOADER_SUCCESS)
    {
        ESP_LOGE(TAG, "failed to change baud rate to %d with error %s", HIGH_BAUD_RATE, esp_err_to_name(err));
        return err;
    }

    target_chip_t target = esp_loader_get_target();
    if (target != ESP32C6_CHIP) {
        ESP_LOGE(TAG, "wrong target %d, expecting ESP32C6_CHIP=%d", target, ESP32C6_CHIP);
        return ESP_LOADER_ERROR_INVALID_TARGET;
    }
    ESP_LOGI(TAG, "Successfully connected to ESP32C6_CHIP %s", "ok");

    return ESP_LOADER_SUCCESS;
}

esp_loader_error_t flash_c6_fw(uint32_t offset, uint32_t image_size, uint8_t* image_data)
{
    constexpr uint32_t block_size = 4*1024;

    ESP_LOGI(TAG, "Flashing %zu bytes with %u block size to offset %x", image_size, block_size, offset);

    esp_loader_error_t err = esp_loader_flash_start(offset, image_size, block_size);
    while (err != ESP_LOADER_SUCCESS)
    {
        ESP_LOGE(TAG, "esp_loader_flash_start failed with %d", err);
        err = esp_loader_flash_start(offset, image_size, block_size);
    }

    uint32_t flashed = 0;
    auto buf = (uint8_t*)malloc(block_size);
    assert(buf != NULL);
    while (flashed < image_size)
    {
        uint32_t to_write = MIN(block_size, image_size-flashed);
        memcpy(buf, image_data+flashed, to_write);

        err = esp_loader_flash_write(buf, to_write);
        if (err != ESP_LOADER_SUCCESS)
        {
            ESP_LOGE(TAG, "esp_loader_flash_write failed with code %d at %d", err, flashed);
            return err;
        }
        flashed += to_write;
        // ESP_LOGI(TAG, "Flashed %d bytes, %d left", flashed, image_size - flashed);
    }
    ESP_LOGI(TAG, "Done flashing %d bytes", flashed);
    free(buf);
    esp_loader_flash_finish(false);

    return ESP_LOADER_SUCCESS;
}

esp_loader_error_t flash_bootloader()
{
    uint32_t size = c6_bootloader_bin_end - c6_bootloader_bin_start;

    auto result = esp_loader_flash_verify_known_md5(c6_bootloader_offset, size, c6_bootloader_bin_md5);
    if (result != ESP_LOADER_SUCCESS)
    {
        return flash_c6_fw(c6_bootloader_offset, size, (uint8_t*)c6_bootloader_bin_start);
    }
    return ESP_LOADER_SUCCESS;
}

esp_loader_error_t flash_boot_app0()
{
    uint32_t size = c6_boot_app0_bin_end - c6_boot_app0_bin_start;

    auto result = esp_loader_flash_verify_known_md5(c6_boot_app0_offset, size, c6_boot_app0_bin_md5);
    if (result != ESP_LOADER_SUCCESS)
    {
        return flash_c6_fw(c6_boot_app0_offset, size, (uint8_t*)c6_boot_app0_bin_start);
    }
    return ESP_LOADER_SUCCESS;
}

esp_loader_error_t flash_firmware()
{
    uint32_t size = c6_firmware_bin_end - c6_firmware_bin_start;

    esp_loader_error_t result = esp_loader_flash_verify_known_md5(c6_firmware_offset, size, c6_firmware_bin_md5);
    if (result != ESP_LOADER_SUCCESS)
    {
        return flash_c6_fw(c6_firmware_offset, size, (uint8_t*)c6_firmware_bin_start);
    }
    return ESP_LOADER_SUCCESS;
}

esp_loader_error_t flash_partitions()
{
    uint32_t size = c6_partitions_bin_end - c6_partitions_bin_start;

    auto result = esp_loader_flash_verify_known_md5(c6_partitions_bin_offset, size, c6_partitions_bin_md5);
    if (result != ESP_LOADER_SUCCESS)
    {
        return flash_c6_fw(c6_partitions_bin_offset, size, (uint8_t*)c6_partitions_bin_start);
    }
    return ESP_LOADER_SUCCESS;
}

esp_loader_error_t flash_c6_if_needed()
{
    ESP_LOGI(TAG, "Preparing for flashing FW %s", c6_firmware_bin_md5);
    err = init_connection_for_flashing();
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "init_connection_for_flashing failed with %d", err);
        ESP_ERROR_CHECK(err);
    }
    err = flash_bootloader();
    if (err != ESP_LOADER_SUCCESS)
    {
        ESP_LOGE(TAG, "flash_bootloader failed with %d", err);
        ESP_ERROR_CHECK(err);
    }

    err = flash_partitions();
    if (err != ESP_LOADER_SUCCESS)
    {
        ESP_LOGE(TAG, "flash_partitions failed with %d", err);
        ESP_ERROR_CHECK(err);
    }

    err = flash_boot_app0();
    if (err != ESP_LOADER_SUCCESS)
    {
        ESP_LOGE(TAG, "flash_boot_app0 failed with %d", err);
        ESP_ERROR_CHECK(err);
    }

    err = flash_firmware();
    if (err != ESP_LOADER_SUCCESS)
    {
        ESP_LOGE(TAG, "flash_firmware failed with %d", err);
        ESP_ERROR_CHECK(err);
    }

    ESP_LOGI(TAG, "flashing all OK, resetting C6 and waiting %d ms", after_reboot_delay_ms);
    esp_loader_reset_target();
    delay(after_reboot_delay_ms);
    return ESP_LOADER_SUCCESS;
}
