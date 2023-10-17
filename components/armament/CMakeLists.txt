idf_component_register(
    SRCS "arma_output.c" "arma_deauth.c" "arma_mic.c" "armament.c" "arma_pmkid.c" "arma_mic.c" "arma_reconnaissance.c" "arma_utils.c"
    INCLUDE_DIRS "includes" "interface"
    PRIV_REQUIRES esp_wifi wifi_ctl frame_parser
)