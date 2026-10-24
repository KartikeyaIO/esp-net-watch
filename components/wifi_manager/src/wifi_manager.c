#include "esp_err.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_wifi_types_generic.h"
#include "nvs_flash.h"
#include <stdint.h>
#include <string.h>
#include "wifi_manager.h"



/*
 * @brief Initializes The NVS and wifi for configurations call this function first..*/

wifi_err initialize_wifi(){
  esp_err_t e;
  e = nvs_flash_init();
  if (e != ESP_OK){
    return NVSFailed;
  }
  wifi_init_config_t config = WIFI_INIT_CONFIG_DEFAULT();
  e = esp_wifi_init(&config);
  if (e != ESP_OK ){
    return InitializeFailed;
  }
  return WifiOK;
}


/*
 * @brief Configures Wifi for Access Point (AP) mode
 * */

wifi_err configure_ap(uint8_t * ssid, uint8_t *password){
  esp_err_t e;
  wifi_mode_t mode = WIFI_MODE_AP ;
  wifi_interface_t interface = WIFI_IF_AP;
  wifi_config_t config;
  memcpy(config.ap.ssid, ssid, 32);
  memcpy(config.ap.password, password, 64);
  e = esp_wifi_set_mode(mode);
  if (e == ESP_ERR_WIFI_NOT_INIT ){
    return WifiNotInitialised;
  }
  e = esp_wifi_set_config(interface, &config);
  if (e != ESP_OK){
    return ConfigFailed;
  }
  return WifiOK;


}

/*
 * @brief Configures Wifi for Station (STA) mode
 * */

wifi_err configure_sta(uint8_t * ssid, uint8_t *password){
  esp_err_t e;
  wifi_mode_t mode = WIFI_MODE_STA ;
  wifi_interface_t interface = WIFI_IF_STA;
  wifi_config_t config;
  memcpy(config.sta.ssid, ssid, 32);
  memcpy(config.sta.password, password, 64);
  e = esp_wifi_set_mode(mode);
  if (e == ESP_ERR_WIFI_NOT_INIT ){
    return WifiNotInitialised;
  }
  e = esp_wifi_set_config(interface, &config);
  if (e != ESP_OK){
    return ConfigFailed;
  }
  return WifiOK;


}

/*
 * @brief Configures Wifi for AP-STA mode
 * */

wifi_err configure_ap_sta(uint8_t * ssid_ap, uint8_t *password_ap, uint8_t *ssid_sta, uint8_t *password_sta){
  wifi_err e;
  e = configure_ap(ssid_ap, password_ap);
  if (e != WifiOK){
    return ConfigFailed;
  }
  e = configure_sta(ssid_sta,password_sta);
  if (e != WifiOK){
    return ConfigFailed;
  }
  return WifiOK;

}



// Function to connect to an Access point

wifi_err connect_to_ap(uint8_t * ssid, uint8_t * password){
    wifi_err e;
    esp_err_t err;
    e = configure_sta(ssid, password);
    if (e != WifiOK){
      return e;
    }
    err = esp_wifi_start();
    if (err == ESP_ERR_NO_MEM ){
      return OutOfMemory;
    }
    if (err == ESP_ERR_WIFI_CONN){
      return DriverError;
    }
    err = esp_wifi_connect();
    if (err == ESP_ERR_WIFI_NOT_STARTED ){
      return WifiStartFailed;
    }
    if (err == ESP_ERR_WIFI_CONN){
      return DriverError;
    }
    if (err == ESP_ERR_WIFI_SSID){
      return InvalidSSID;
    }
    return WifiOK;



}
