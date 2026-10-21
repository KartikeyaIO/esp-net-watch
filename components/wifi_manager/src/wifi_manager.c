#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_wifi_types.h"
#include <stdint.h>
#include<stdint.h>

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
    WifiNotInitialised,
    InitializeFailed,
    WifiDriverFailed,
    WifiAuthFailed,
    WifiNotFound,
    ConfigSuccess,
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


/*
It starts the wifi drivers configure it for the chosen mode with credentials, the functions expects the following:
Mode, ssid and password
** mode: Use WifiMode Enum to select the Mode
** ssid: Your AP's SSID..
** password: you guessed it, it's the password. No Hidden meanings there.
** @Note The SSID is 32B and Password is 64B, as per the standard ESP-IDF config structures.. therefore if you enter any value larger than this, it WILL NOT raise any error but the ssid and passwords will be clipped.
it returns wifi_err enum for error handling

*/

wifi_err configure_wifi(WifiMode mode,uint8_t *ssid, uint8_t *password){
    uint8_t ssid_final[32];
    uint8_t pass_final[64];
    wifi_interface_t interface;
    wifi_mode_t final_mode;
    wifi_config config;
    for (int i=0; i<32; i++){
        if (ssid[i] == '\0') {
            ssid_final[i] = '\0';
            break;
        }
        ssid_final[i] = ssid[i];
    }
    for (int i=0; i<64; i++){
        if (password[i] == '\0') {
            pass_final[i] = '\0';
            break;
        }
        pass_final[i] = password[i];
    }
    switch (mode){

        case WifiAP: {
            wifi_ap_config_t value = {.ssid = ssid_final, .password = pass_final};
            config.ap_config = value;
            final_mode = WIFI_MODE_AP;
            interface = WIFI_IF_AP;
            break;
        }
        case WifiSTA: {
            wifi_sta_config_t value = {.ssid = ssid_final, .password = pass_final};
            config.sta_config = value;
            final_mode = WIFI_MODE_STA;
            interface = WIFI_IF_STA;
            break;
        }
        case WifiAPSTA: {
            wifi_ap_config_t ap_config = {.ssid = ssid_final, .password = pass_final};
            wifi_sta_config_t sta_config = {.ssid = ssid_final, .password = pass_final};
            wifi_apsta_config_t final = {.sta_config = sta_config, .ap_config = ap_config};
            config.ap_sta_config = final;
            final_mode = WIFI_MODE_APSTA;
            break;
        }
        default: {
            return InvalidMode;
        }
    };

    esp_err_t e;
    e = esp_wifi_init(WIFI_INIT_CONFIG_DEFAULT());
    if (e != ESP_OK){
        return InitializeFailed;
    }

    e = esp_wifi_set_mode(final_mode);
    if (e!=ESP_OK){return InvalidMode;}

    // sets up configurations for the wifi driver, after this the connect or scan functions maybe called
    e = esp_wifi_set_config(interface,&config);
    if (e != ESP_OK){
    return ConfigFailed;
    }
    return ConfigSuccess;


}

// Function to connect to an Access point

wifi_err connect_to_ap(uint8_t * ssid, uint8_t * password){
    wifi_err e;
    e = configure_wifi(WifiSTA, ssid, password)
    if (e == )
}


