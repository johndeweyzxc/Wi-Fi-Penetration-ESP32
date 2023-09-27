idf_component_register(
    SRCS "cmd_parser.c" 
    INCLUDE_DIRS "includes" "interface"
    PRIV_REQUIRES armament esp_event
)