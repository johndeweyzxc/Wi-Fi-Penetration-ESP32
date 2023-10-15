idf_component_register(
    SRCS "wifi_ctl.c" 
    INCLUDE_DIRS "includes" "interface"
    PRIV_REQUIRES esp_event esp_netif esp_wifi frame_bypasser
)