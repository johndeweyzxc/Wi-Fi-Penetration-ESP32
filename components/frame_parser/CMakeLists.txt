idf_component_register(
    SRCS "frame_output.c" "frame_parser.c" "eapol_validator.c"
    INCLUDE_DIRS "includes" "frame_types" "interface"
    PRIV_REQUIRES esp_event esp_wifi
)