#include "esp_loader.h"

esp_loader_error_t err;


const uint8_t* c6_fw_start = asm("_binary_firmware_why2025_carrier_start");
const uint8_t* c6_fw_end = asm("_binary_firmware_why2025_carrier_end");

void init_flash() {

}

void flash_c6_if_needed() {

}