#include <esp_loader.h>
#include <esp32_port.h>
#include <esp_err.h>
#include <driver/uart.h>
#include <driver/GPIO.h>
#include <esp_log.h>

esp_err_t err;


extern const uint8_t* c6_fw_start asm("_binary_firmware_why2025_carrier_bin_start");
extern const uint8_t* c6_fw_end asm("_binary_firmware_why2025_carrier_bin_end");
extern const uint8_t* c6_fw_md5 asm("_binary_firmware_why2025_carrier_bin_md5_start");

const char* TAG = "c6_fw_flasher";

constexpr int DEFAULT_BAUD_RATE = 115200;
// constexpr int HIGH_BAUD_RATE = 460800;

esp_err_t init_connection_for_flashing()
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
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "loader_port_esp32_init failed with error %s", esp_err_to_name(err));
        return err;
    }

    esp_loader_connect_args_t esp_loader_connect_args = ESP_LOADER_CONNECT_DEFAULT();
    err = esp_loader_connect_with_stub(&esp_loader_connect_args);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "esp_loader_connect_with_stub failed with error %s", esp_err_to_name(err));
        return err;
    }

    // ESP_LOGI(TAG, "Changing baud rate from %d to %d", DEFAULT_BAUD_RATE, HIGH_BAUD_RATE);
    // err = esp_loader_change_transmission_rate_stub(DEFAULT_BAUD_RATE, HIGH_BAUD_RATE);
    // if (err != ESP_OK)
    // {
    //     ESP_LOGE(TAG, "failed to change baud rate to %d with error %s", HIGH_BAUD_RATE, esp_err_to_name(err));
    //     return err;
    // }

    target_chip_t target = esp_loader_get_target();
    if (target != ESP32C6_CHIP) {
        ESP_LOGE(TAG, "wrong target %d, expecting ESP32C6_CHIP=%d", target, ESP32C6_CHIP);
        return ESP_FAIL;
    }

    return ESP_OK;
}

esp_loader_error_t check_if_flash_needed()
{
    const uint32_t image_size = c6_fw_end - c6_fw_start;
    return esp_loader_flash_verify_known_md5(0x00, image_size, c6_fw_md5);
}

void flash_c6_fw()
{
    const uint32_t image_size = c6_fw_end - c6_fw_start;
    ESP_LOGI(TAG, "Flashing %zu bytes", image_size);
    const uint8_t block_size = 1024;
    esp_loader_flash_start(0x00, image_size, block_size);
    uint32_t flashed = 0;
    while (flashed < image_size)
    {
        err = esp_loader_flash_write((void*)(c6_fw_start+flashed), block_size);
        ESP_ERROR_CHECK(err);
        ESP_LOGI(TAG, "Flashed %zu bytes", flashed);
        flashed += block_size;
    }
    ESP_LOGI(TAG, "Done flashing %zu bytes", flashed);
    esp_loader_flash_finish(true);

    ESP_ERROR_CHECK(err);

}

void flash_c6_if_needed()
{
    err = init_connection_for_flashing();
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "init_connection_for_flashing failed with %d", err);
        ESP_ERROR_CHECK(err);
    }
    esp_err_t err = check_if_flash_needed();
    if (err != ESP_LOADER_SUCCESS)
    {
        flash_c6_fw();
    }
}
