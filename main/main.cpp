
#include "Arduino.h"
#include "esp_loader.h"
#include "Log.h"
#include "comms/EthClient.h"
#include "comms/UARTClient.h"
#include "graphics/DeviceScreen.h"


#include "LittleFS.h"
#define FSCom LittleFS
#define FSBegin() LittleFS.begin(true)

#if defined(I2C_SDA) || defined(I2C_SDA1)
#include "Wire.h"
#endif

// this is pulled in by the device-ui library
extern const uint8_t fw_version_start[] asm("_binary_meshtastic_fw_version_start");

const char *firmware_version = (char*)fw_version_start;

IClientBase *client = nullptr;
DeviceScreen *screen = nullptr;

esp_loader_error_t flash_c6_if_needed();

void setup()
{
    esp_loader_error_t esp_loader_error = flash_c6_if_needed();
    if (esp_loader_error != ESP_LOADER_SUCCESS)
    {
        ILOG_CRIT("failed to flash c6 with error %d", esp_loader_error);
        return;
    }
#ifndef USE_SERIAL0
#ifdef WAIT_FOR_SERIAL0
    delay(2000);
#endif
    Serial.begin(115200);
#ifdef WAIT_FOR_SERIAL0
    time_t timeout = millis();
    while (!Serial && (millis() - timeout) < 2000)
        ;
#endif
    logger.setDebugLevel(ESP_LOG_DEBUG); // use ESP_LOG_VERBOSE for trace category
#else
    logger.setDebugLevel(ESP_LOG_NONE); // do not log when connected over serial0
#endif

#ifdef I2C_SDA
    if (!Wire.begin(I2C_SDA, I2C_SCL, 400000))
        ILOG_ERROR("*** Failed to access I2C0(%d, %d)", I2C_SDA, I2C_SCL);
#endif
#ifdef I2C_SDA1
    if (!Wire.begin(I2C_SDA1, I2C_SCL1, 400000))
        ILOG_ERROR("*** Failed to access I2C1(%d, %d)", I2C_SDA1, I2C_SCL1);
#endif

    ILOG_INFO("\n//\\ E S H T /\\ S T / C   U I\n");

    uint64_t chipid;
    chipid = ESP.getEfuseMac(); // The chip ID is essentially its MAC address(length: 6 bytes).
    ILOG_DEBUG("  ESP32 Chip ID = %04X %08X", (uint16_t)(chipid >> 32), (uint32_t)chipid);
    ILOG_DEBUG("  Flash size: %8d bytes", ESP.getFlashChipSize());
    ILOG_DEBUG("  Heap size : %8d bytes", ESP.getHeapSize());
    ILOG_DEBUG("  Free heap : %8d bytes", ESP.getFreeHeap());
    ILOG_DEBUG("  PSRAM     : %8d bytes", ESP.getFreePsram());
    ILOG_DEBUG("  PSRAM max : %8d bytes", heap_caps_get_largest_free_block(MALLOC_CAP_SPIRAM));
    ILOG_DEBUG("*****************************************");

    if (!FSBegin()) {
        ILOG_ERROR("LittleFS mount failed!");
    }

    client = new UARTClient();
    screen = &DeviceScreen::create(DisplayDriverConfig(DisplayDriverConfig::device_t::WHY2025_M2, 720, 720));

    screen->init(client);

    ILOG_DEBUG("Free heap : %8d bytes", ESP.getFreeHeap());
    ILOG_DEBUG("PSRAM     : %8d bytes", ESP.getFreePsram());

}

void loop()
{
    screen->task_handler();
    screen->sleep(5);
}
