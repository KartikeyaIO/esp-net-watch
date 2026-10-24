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
  * @return
  *     - WifiOK: Success
  *     - NVSFailed: NVS Initialization Failed
  *     - InitializeFailed:Initialization Failed at the esp_wifi_init() step.
*/

wifi_err initialize_wifi(void);


/** @brief Configures Wifi for AP (Access Point) Mode
 * @param ssid SSID you want for the AP mode
 * @param password Password.
 * @return
 *      - WifiOK: Success
 *      - WifiNotInitialised: Wifi was not initialized before calling this function
 *      - ConfigFailed: Wifi Configuration Failed.
 * @attention SSID and Password is 32B and 64B respectively, and are truncated if the provided array is bigger
 */

wifi_err configure_ap(const uint8_t *ssid, const uint8_t * password);

/** @brief Configures Wifi for STA (Station) Mode
 * @param ssid SSID you want for the AP mode
 * @param password Password.
 * @return
 *      - WifiOK; Success
 *      - WifiNotInitialised: Wifi was not initialized before calling this function
 *      - ConfigFailed: Configuration Failed for the Driver
 * @attention SSID and Password is 32B and 64B respectively, and are truncated if the provided array is bigger
 */



wifi_err configure_sta(const uint8_t *ssid, const uint8_t * password);

/** @brief Configures Wifi for AP-STA Mode
 * @param ssid SSID you want for the AP mode
 * @param password Password.
 * @return
 *      - WifiOK: Success
 *      - returns the errors of config_sta and config_ap as it is.
 *
 * @attention SSID and Password is 32B and 64B respectively, and are truncated if the provided array is bigger
 */



wifi_err configure_ap_sta(const uint8_t * ssid_ap, const uint8_t *password_ap, const uint8_t *ssid_sta, const uint8_t *password_sta);


/**
  * @brief Connects to an Access Point with the provided SSID and Password
  *
  * @param ssid SSID of the AP.
  * @param password Password
  *
  * @return
  *     - WifiOK: Success
  *     - OutOfMemory: Memory is full
  *     - DriverError: Internal Error, equivalent to ESP_ERR_WIFI_CONN
  *     - InvalidSSID: SSID cannot be located
  *     - WifiStartFailed: Something Failed when Starting the Driver.
  *     - WifiNotInitialised: Wifi was not initialized before calling this function.
  *     - ConfigFailed: Returned by the configure_sta
  * @attention The SSID is 32B and Password is 64B, as per the standard ESP-IDF config structures. Therefore if the values are bigger than that they will be clipped.
*/
wifi_err connect_to_ap(const uint8_t *ssid, const uint8_t * password);

#endif
