#include <stdint.h>
#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "wifi_manager.h"


void app_main(void)
{
    printf("\nWiFi Manager test starting...\n");

    wifi_err e = initialize_wifi();

    if (e != WifiOK) {
        printf("WiFi initialization failed: error=%d\n", (int)e);
        return;
    }

    printf("WiFi initialized successfully.\n");

    const uint8_t *ssid = (const uint8_t *)"Samsung M14 5G";
    const uint8_t *password = (const uint8_t *)"Password";

    e = connect_to_ap(ssid, password);
    vTaskDelay(pdMS_TO_TICKS(1500));

    if (e != WifiOK) {
        printf("Connection setup failed: error=%d\n", (int)e);
    } else {
        printf("Connection request accepted.\n");
    }

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
