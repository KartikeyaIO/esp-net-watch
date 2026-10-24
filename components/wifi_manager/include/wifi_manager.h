#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H
// Wifi_Manager.h public header for wifi_manager.c

#include "esp_err.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_wifi_types.h"
#include <stdint.h>

// Defines the Modes of operations
typedef enum
{
    WifiSTA,
    WifiAP,
    WifiAPSTA,
    WifiPromiscuous,

} WifiMode;

// Defines the errors for the wifi manager library.
// Redesign of errors is needed
typedef enum
{
    WifiOK,
    WifiNotInitialised,
    InitializeFailed,
    WifiDriverFailed,
    WifiAuthFailed,
    WifiNotFound,
    OutOfMemory,
    WifiStartFailed,
    DriverError,
    InvalidSSID,
    NVSFailed,
    ConfigFailed,
    InvalidMode,
} wifi_err;

// struct to store the complete config for AP-STA mode..
typedef struct {
    wifi_sta_config_t sta_config;
    wifi_ap_config_t ap_config;
} wifi_apsta_config_t;

// Union that allows us to setup the cofig.
typedef union {
    wifi_sta_config_t sta_config;
    wifi_ap_config_t ap_config;
    wifi_apsta_config_t ap_sta_config;
} wifi_config;




// Functions

/**
  * @brief Initializes the NVS and wifi, Call this as the first step.
*/

wifi_err initialize_wifi(void);




/**
  * @brief Connects to an Access Point with the provided SSID and Password
  *
  * @param ssid SSID of the AP.
  * @param password Password
  *
  * @return
  *     - WifiOK: Success
  *     - InvalidMode: if wrong mode is selected
  *     - InitializeFailed: Failed at init step, Something might be wrong with the RAM or driver startup.
  *     - ConfigFailed: Failed at config setup, Configuration might be wrong
  *     - WifiDriverFailed: Failed at the Driver Startup
  *     - APConnectFailed: Failed at the connect step.
  * @attention The SSID is 32B and Password is 64B, as per the standard ESP-IDF config structures. Therefore if the values are bigger than that they will be clipped.
*/
wifi_err connect_to_ap(uint8_t *ssid, uint8_t * password);

#endif
