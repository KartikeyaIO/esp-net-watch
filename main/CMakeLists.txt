idf_component_register(SRCS "esp-net-watcher.c"
                       PRIV_REQUIRES spi_flash nvs_flash esp_wifi
                       INCLUDE_DIRS "."
                       REQUIRES esp_wifi)
