idf_component_register(
    SRCS "armament.c"
    INCLUDE_DIRS "includes" "interface"
    PRIV_REQUIRES esp_wifi wifi_ctl frame_parser
)