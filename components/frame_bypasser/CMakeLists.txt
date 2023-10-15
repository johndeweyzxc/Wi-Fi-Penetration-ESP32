idf_component_register(
    SRCS "frame_bypasser.c" 
    INCLUDE_DIRS "includes" "interface"
    PRIV_REQUIRES esp_wifi
)

target_link_libraries(${COMPONENT_LIB} -Wl,-zmuldefs)