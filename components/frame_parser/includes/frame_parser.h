#pragma once
#include <stdint.h>

#include "eapol_frame.h"
#include "frame_constants.h"

enum parse_type_list { NULL_PARSE_TYPE, PARSE_PMKID, PARSE_MIC };

enum frame_event_id { DATA_FRAME, MGMT_FRAME, CTRL_FRAME };

enum arma_atk_notify_status_event_id { ATK_STATS_EVENT_ID };

enum arma_atk_notify_context { PMKID_BASED, MIC_BASED };

typedef struct {
  uint8_t atk_context;
} arma_atk_event_data_t;

void post_pmkid_attack_notification();

void parse_pmkid(eapol_auth_data_t *wpa_data, eapol_frame_t *eapol_frame,
                 key_information_t *key_info);

void parse_mic(eapol_auth_data_t *wpa_data, eapol_frame_t *eapol_frame,
               key_information_t *key_info);

key_information_t perform_bitwise_on_key_info(uint16_t *key_info_16);

void parse_80211_authentication(eapol_auth_data_t *wpa_data,
                                eapol_frame_t *eapol_frame);

void frame_parser_unregister_data_frame_handler();

void frame_parser_register_data_frame_handler();

void frame_parser_clear_target_param();

void frame_parser_set_target_parameter(uint8_t *target_bssid,
                                       uint8_t selected_parse_type);