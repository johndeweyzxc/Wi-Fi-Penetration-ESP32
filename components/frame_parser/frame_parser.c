/*
 * @file frame_parser.c
 * @author johndeweyzxc (johndewey02003@gmail.com)
 * @brief Implements functionality for parsing frames of type data
 */

#include "frame_parser.h"

#include <stdio.h>

#include "arpa/inet.h"
#include "eapol_frame.h"
#include "eapol_validator.h"
#include "esp_err.h"
#include "esp_event.h"
#include "esp_wifi_types.h"
#include "frame_output.h"

ESP_EVENT_DEFINE_BASE(FRAME_RECEIVED_EVENT_BASE);
ESP_EVENT_DEFINE_BASE(ARMAMENT_ATTACK_STATUS_EVENT_BASE);
static uint8_t *bssid = NULL;
static uint8_t parse_type = NULL_PARSE_TYPE;

void pmkid_notify_armament() {
  printf("frame_parser.pmkid_notify_armament > *\n");
  arma_atk_event_data_t event_data;
  event_data.atk_context = PMKID_BASED;
  ESP_ERROR_CHECK(esp_event_post(ARMAMENT_ATTACK_STATUS_EVENT_BASE,
                                 ATK_STATS_EVENT_ID, &event_data,
                                 sizeof(event_data), portMAX_DELAY));
}

void parse_pmkid(eapol_auth_data_t *wpa_data, eapol_frame_t *eapol_frame,
                 key_information_t *key_info) {
  if (key_info->key_type == 1 && key_info->key_ack == 1 &&
      key_info->install == 0) {
    wpa_key_data_t *key_data = (wpa_key_data_t *)wpa_data->wpa_key_data;
    if (eapol_valid_pmkid(key_data) == GOOD_PMKID) {
      output_pmkid(eapol_frame);
      pmkid_notify_armament();
    } else {
      return;
    }
  } else {
    printf("frame_parser.parse_pmkid > This is not msg 1\n");
  }
}

void parse_mic(eapol_auth_data_t *wpa_data, eapol_frame_t *eapol_frame,
               key_information_t *key_info) {
  if (key_info->key_type == 1 && key_info->key_ack == 1 &&
      key_info->install == 0) {
    output_anonce_from_message_1(eapol_frame);
  } else {
    printf("frame_parser.parse_mic > This is not msg 1\n");
  }

  if (key_info->key_type == 1 && key_info->key_mic == 1 &&
      key_info->secure == 0) {
    output_mic_from_message_2(eapol_frame);
  } else {
    printf("frame_parser.parse_mic > This is not msg 2\n");
  }
}

key_information_t perform_bitwise_on_key_info(uint16_t *key_info_16) {
  key_information_t key_info;

  key_info.reserved = ((*key_info_16) >> 14) & 0x03;
  key_info.smk_message = ((*key_info_16) >> 13) & 0x01;
  key_info.encrypted_key_data = ((*key_info_16) >> 12) & 0x01;
  key_info.request = ((*key_info_16) >> 11) & 0x01;
  key_info.error = ((*key_info_16) >> 10) & 0x01;
  key_info.secure = ((*key_info_16) >> 9) & 0x01;
  key_info.key_mic = ((*key_info_16) >> 8) & 0x01;
  key_info.key_ack = ((*key_info_16) >> 7) & 0x01;
  key_info.install = ((*key_info_16) >> 6) & 0x01;
  key_info.key_index = ((*key_info_16) >> 4) & 0x03;
  key_info.key_type = ((*key_info_16) >> 3) & 0x01;
  key_info.key_descriptor_version = (*key_info_16) & 0x07;

  return key_info;
}

void parse_80211_authentication(eapol_auth_data_t *wpa_data,
                                eapol_frame_t *eapol_frame) {
  uint16_t key_info_16 = htons(wpa_data->key_info);
  key_information_t key_info = perform_bitwise_on_key_info(&key_info_16);

  if (parse_type == PARSE_PMKID) {
    parse_pmkid(wpa_data, eapol_frame, &key_info);
  } else if (parse_type == PARSE_MIC) {
    parse_mic(wpa_data, eapol_frame, &key_info);
  }
}

static void data_frame_parser(void *args, esp_event_base_t event_base,
                              int32_t event_id, void *event_data) {
  wifi_promiscuous_pkt_t *frame = (wifi_promiscuous_pkt_t *)event_data;
  eapol_frame_t *eapol_frame = (eapol_frame_t *)frame->payload;
  frame_control_t *frame_control = &eapol_frame->frame_control;
  eapol_auth_data_t *wpa_data = (eapol_auth_data_t *)eapol_frame->auth_data;

  // Checks if it is a IEEE 802.11 QoS data
  if (is_qos_data(frame_control) == NOT_QOS) {
    return;
  }
  // Checks if bssid in eapol frame matched with target bssid
  if (bssid_in_eapol_matched(eapol_frame, bssid) == BSSID_NOT_MATCHED) {
    return;
  }
  // Checks if authentication type is Authentication (0x888e)
  if (is_eapol_auth_type(eapol_frame) == WRONG_EAPOL_AUTH_TYPE) {
    return;
  }
  // Checks if packet type is Key (0x03)
  if (wpa_data->type == EAPOL_KEY_TYPE) {
    parse_80211_authentication(wpa_data, eapol_frame);
  } else {
    printf("frame_parser.data_frame_parser > Wrong packet type: %x\n",
           wpa_data->type);
  }
}

void frame_parser_unregister_data_frame_handler() {
  ESP_ERROR_CHECK(esp_event_handler_unregister(FRAME_RECEIVED_EVENT_BASE,
                                               DATA_FRAME, &data_frame_parser));
  printf("frame_parser.frame_parser_unregister_data_frame_handler > *\n");
}

void frame_parser_register_data_frame_handler() {
  ESP_ERROR_CHECK(esp_event_handler_register(
      FRAME_RECEIVED_EVENT_BASE, DATA_FRAME, &data_frame_parser, NULL));
  printf("frame_parser.frame_parser_register_data_frame_handler > *\n");
}

void frame_parser_clear_target_parameter() {
  bssid = NULL;
  parse_type = NULL_PARSE_TYPE;
  printf("frame_parser.frame_parser_clear_target_parameter > *\n");
}

void frame_parser_set_target_parameter(uint8_t *target_bssid,
                                       uint8_t selected_parse_type) {
  bssid = target_bssid;
  parse_type = selected_parse_type;
  printf("frame_parser.frame_parser_set_target_parameter > *\n");
}