#include <stdio.h>
#include "esp_wifi.h"
#include "esp_err.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_netif.h"
#include "lwip/ip4_addr.h"

#define MAX_AP 20

void scan_wifi(void)
{
    wifi_scan_config_t scan_config = {
        .ssid = NULL,
        .bssid = NULL,
        .channel = 0,
        .show_hidden = true};

    esp_err_t err = esp_wifi_scan_start(&scan_config, true);

    if (err != ESP_OK)
    {
        printf("WiFi scan failed: %s\n",
               esp_err_to_name(err));
        return;
    }

    uint16_t ap_count = 0;

    err = esp_wifi_scan_get_ap_num(&ap_count);

    if (err != ESP_OK)
    {
        printf("Failed to get AP count: %s\n",
               esp_err_to_name(err));
        return;
    }

    if (ap_count > MAX_AP)
    {
        ap_count = MAX_AP;
    }

    wifi_ap_record_t ap_records[MAX_AP];

    err = esp_wifi_scan_get_ap_records(&ap_count, ap_records);
    vTaskDelay(pdMS_TO_TICKS(5000));

    if (err != ESP_OK)
    {
        printf("Failed to get AP records: %s\n",
               esp_err_to_name(err));
        return;
    }

    printf("\nFound %u access points:\n\n", ap_count);

    for (int i = 0; i < ap_count; i++)
    {
        printf("AP %d\n", i + 1);

        printf("  SSID: %s\n", ap_records[i].ssid);

        printf("  BSSID: %02X:%02X:%02X:%02X:%02X:%02X\n",
               ap_records[i].bssid[0],
               ap_records[i].bssid[1],
               ap_records[i].bssid[2],
               ap_records[i].bssid[3],
               ap_records[i].bssid[4],
               ap_records[i].bssid[5]);

        printf("  Channel: %d\n", ap_records[i].primary);

        printf("  RSSI: %d dBm\n", ap_records[i].rssi);

        printf("  Auth mode: %d\n", ap_records[i].authmode);

        printf("\n");
    }
}

void app_main(void)
{
    // Non-Volatile Memory initialization
    esp_err_t err = nvs_flash_init();
    // wifi configuration for STA mode
    wifi_config_t wifi_config = {
        .sta = {.ssid = "Samsung M14 5G", .password = "Password"},
    };

    if (err != ESP_OK)
    {
        printf("NVS initialization failed: %s\n", esp_err_to_name(err));
        return;
    }
    wifi_init_config_t config = WIFI_INIT_CONFIG_DEFAULT();
    // Wifi Initialization
    esp_err_t initializer = esp_wifi_init(&config);
    if (initializer != ESP_OK)
    {
        printf("Wifi Initialization Failed! %s\n", esp_err_to_name(initializer));
        return;
    }
    else
    {
        printf("Wifi Initialization Sucessfull!");
    }
    // Mode change from default mode which came as AP for my case to STA.
    err = esp_wifi_set_mode(WIFI_MODE_STA);
    if (err != ESP_OK)
    {
        printf("Wifi Mode Change to Station failed : %s\n", esp_err_to_name(err));
    }
    else
    {
        printf("Mode change successfull\n");
    }

    wifi_mode_t mode;
    // mode check to know if the change worked or not
    esp_wifi_get_mode(&mode);

    switch (mode)
    {
    case WIFI_MODE_NULL:
        printf("WiFi Mode: NULL\n");
        break;

    case WIFI_MODE_STA:
        printf("WiFi Mode: STA\n");
        break;

    case WIFI_MODE_AP:
        printf("WiFi Mode: AP\n");
        break;

    case WIFI_MODE_APSTA:
        printf("WiFi Mode: AP+STA\n");
        break;

    default:
        printf("WiFi Mode: Unknown\n");
        break;
    }
    err = esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    if (err != ESP_OK)
    {
        printf("Wifi Configuration Failed: %s\n", esp_err_to_name(err));
        return;
    }
    else
    {
        printf("Wifi Initialization Successfull!!!");
    }

    err = esp_wifi_start();
    if (err != ESP_OK)
    {
        printf("Wifi Start Failed: %s\n", esp_err_to_name(err));
        return;
    }
    else
    {
        printf("Wifi has been Started.. Rise and Shine...;)\n");
    }
    scan_wifi();

    err = esp_wifi_connect();
    if (err != ESP_OK)
    {
        printf("Wifi Connection Failed!: %s\n", esp_err_to_name(err));
    }
    else
    {
        printf("Initiating Wifi Connection...\n");
    }
    vTaskDelay(pdMS_TO_TICKS(5000));

    wifi_ap_record_t ap_info;

    err = esp_wifi_sta_get_ap_info(&ap_info);

    if (err != ESP_OK)
    {
        printf("Could not get AP information: %s\n",
               esp_err_to_name(err));
        return;
    }
    printf("Connection to: \n");

    printf("SSID: %s\n", ap_info.ssid);

    printf("BSSID: %02X:%02X:%02X:%02X:%02X:%02X\n",
           ap_info.bssid[0],
           ap_info.bssid[1],
           ap_info.bssid[2],
           ap_info.bssid[3],
           ap_info.bssid[4],
           ap_info.bssid[5]);

    printf("Channel: %d\n", ap_info.primary);

    printf("RSSI: %d dBm\n", ap_info.rssi);
    printf("Successful\n");
    esp_netif_t *netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");

    esp_netif_ip_info_t ip_info;

    err = esp_netif_get_ip_info(netif, &ip_info);

    if (err == ESP_OK)
    {
        printf("IP Address : " IPSTR "\n", IP2STR(&ip_info.ip));
        printf("Netmask    : " IPSTR "\n", IP2STR(&ip_info.netmask));
        printf("Gateway    : " IPSTR "\n", IP2STR(&ip_info.gw));
    }
    else
    {
        printf("No IP info available: %s\n", esp_err_to_name(err));
    }
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
