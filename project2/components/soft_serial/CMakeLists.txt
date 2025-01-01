set(srcs
    "src/sw_serial.c")

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS "include"
                    PRIV_REQUIRES nvs_flash driver esp_netif esp_timer app_update
                    REQUIRES config evse modbus logger)