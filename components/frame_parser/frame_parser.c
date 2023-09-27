#include "frame_parser.h"

#include "arpa/inet.h"
#include "eapol_frame.h"
#include "eapol_validator.h"
#include "esp_err.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_wifi_types.h"
#include "frame_output.h"

static const char *TAG = "FRAME_PARSER";
static uint8_t *bssid = NULL;
static uint8_t parse_type = NULL_PARSE_TYPE;
ESP_EVENT_DEFINE_BASE(FRAME_RECEIVED_EVENT_BASE);

void parse_pmkid(eapol_auth_data_t *wpa_data, eapol_frame_t *eapol_frame,
                 key_information_t *key_info) {
  if (key_info->key_type == 1 && key_info->key_ack == 1 &&
      key_info->install == 0) {
    wpa_key_data_t *key_data = (wpa_key_data_t *)wpa_data->wpa_key_data;
    if (eapol_has_valid_pmkid_key_data(key_data) == 0) {
      return;
    } else {
      output_pmkid(eapol_frame);
    }
  } else {
    ESP_LOGI(TAG, "Parsing pmkid, this is not message 1...");
  }
}

void parse_mic(eapol_auth_data_t *wpa_data, eapol_frame_t *eapol_frame,
               key_information_t *key_info) {
  if (key_info->key_type == 1 && key_info->key_ack == 1 &&
      key_info->install == 0) {
    output_mic_message_1(eapol_frame);
  } else {
    ESP_LOGI(TAG, "Parsing mic, this is not message 1...");
  }

  if (key_info->key_type == 1 && key_info->key_mic == 1 &&
      key_info->secure == 0) {
    output_mic_message_2(eapol_frame);
  } else {
    ESP_LOGI(TAG, "Parsing mic, this is not message 2...");
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

static void cb_data_handler(void *args, esp_event_base_t event_base,
                            int32_t event_id, void *event_data) {
  wifi_promiscuous_pkt_t *frame = (wifi_promiscuous_pkt_t *)event_data;
  eapol_frame_t *eapol_frame = (eapol_frame_t *)frame->payload;
  frame_control_t *frame_control = &eapol_frame->frame_control;
  eapol_auth_data_t *wpa_data = (eapol_auth_data_t *)eapol_frame->auth_data;

  // * Checks if it is a IEEE 802.11 QoS data
  if (is_qos_data(frame_control) == NOT_QOS) {
    return;
  }
  // Checks if bssid in eapol frame matched with target bssid
  if (bssid_in_eapol_matched(eapol_frame, bssid) == BSSID_NOT_MATCHED) {
    return;
  }
  // * Checks if authentication type is 0x888e
  if (is_eapol_auth_type(eapol_frame) == WRONG_EAPOL_AUTH_TYPE) {
    return;
  }
  // * Checks if key type is 0x03
  if (wpa_data->type == EAPOL_KEY_TYPE) {
    parse_80211_authentication(wpa_data, eapol_frame);
  } else {
    ESP_LOGW(TAG, "Wrong packet type, got %x...", wpa_data->type);
  }
}

void frame_parser_unregister_data_frame_handler() {
  ESP_ERROR_CHECK(esp_event_handler_unregister(FRAME_RECEIVED_EVENT_BASE,
                                               DATA_FRAME, &cb_data_handler));
  ESP_LOGI(TAG, "Data frame event handler unregistered...");
}

void frame_parser_register_data_frame_handler() {
  ESP_ERROR_CHECK(esp_event_handler_register(
      FRAME_RECEIVED_EVENT_BASE, DATA_FRAME, &cb_data_handler, NULL));
  ESP_LOGI(TAG, "Data frame event handler registered...");
}

void frame_parser_clear_target_param() {
  bssid = NULL;
  parse_type = NULL_PARSE_TYPE;
  ESP_LOGI(TAG, "Cleared target parameter...");
}

void frame_parser_set_target_parameter(uint8_t *target_bssid,
                                       uint8_t selected_parse_type) {
  bssid = target_bssid;
  parse_type = selected_parse_type;
  ESP_LOGI(TAG, "Set target parameter...");
}