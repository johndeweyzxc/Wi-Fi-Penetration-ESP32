#pragma once
#include <stdint.h>

enum parse_type_list { NULL_PARSE_TYPE, PARSE_PMKID, PARSE_MIC };

enum frame_event_id { DATA_FRAME, MGMT_FRAME, CTRL_FRAME };

enum arma_attack_status_event_id { ATK_STATS_EVENT_ID };

enum arma_attack_context { PMKID_BASED, MIC_BASED };

typedef struct {
  uint8_t atk_context;
} arma_atk_event_data_t;

void frame_parser_unregister_data_frame_handler();

void frame_parser_register_data_frame_handler();

void frame_parser_clear_target_param();

void frame_parser_set_target_parameter(uint8_t *target_bssid,
                                       uint8_t selected_parse_type);