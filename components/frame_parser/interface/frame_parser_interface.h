#pragma once
#include <stdint.h>

enum frame_event_id { DATA_FRAME, MGMT_FRAME, CTRL_FRAME };

void frame_parser_unregister_data_frame_handler();

void frame_parser_register_data_frame_handler();

void frame_parser_clear_target_param();

void frame_parser_set_target_parameter(uint8_t *target_bssid,
                                       uint8_t selected_parse_type);