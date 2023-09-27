idf_component_register(
    SRCS "main.c"
    INCLUDE_DIRS "." 
    PRIV_REQUIRES cmd_parser esp_event
)
