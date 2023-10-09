#include "frame_output.h"

#include <stdio.h>

#include "arpa/inet.h"
#include "eapol_frame.h"
#include "frame_constants.h"

void put_mac_addr_in_buff(char *char_buff, uint8_t *addr) {
  sprintf(char_buff, "%02X%02X%02X%02X%02X%02X,", addr[0], addr[1], addr[2],
          addr[3], addr[4], addr[5]);
}

void put_nonce_in_buff(char *char_buff, uint8_t *n) {
  sprintf(char_buff,
          "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
          n[0], n[1], n[2], n[3], n[4], n[5], n[6], n[7], n[8], n[9], n[10],
          n[11], n[12], n[13], n[14], n[15]);
  char_buff += 16;
  sprintf(char_buff,
          "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X,",
          n[16], n[17], n[18], n[19], n[20], n[21], n[22], n[23], n[24], n[25],
          n[26], n[27], n[28], n[29], n[30], n[31]);
}

void put_key_in_buff(char *char_buff, uint8_t *k) {
  sprintf(char_buff,
          "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X,",
          k[0], k[1], k[2], k[3], k[4], k[5], k[6], k[7], k[8], k[9], k[10],
          k[11], k[12], k[13], k[14], k[15]);
}

void put_m2_info_in_buff(char *char_buff, eapol_auth_data_t *auth_message_2) {
  sprintf(char_buff, "%02X,%02X,%02X%02X,%02X,", auth_message_2->version,
          auth_message_2->type, auth_message_2->length[0],
          auth_message_2->length[1], auth_message_2->key_desc_type);
}

void put_key_data_in_buff(char *char_buff, uint8_t *key_data, uint16_t length) {
  for (uint8_t i = 0; i < length; i++) {
    if (i == length - 1) {
      sprintf(char_buff, "%02X,", key_data[i]);
    } else {
      sprintf(char_buff, "%02X", key_data[i]);
      char_buff += 2;
    }
  }
}

void output_pmkid(eapol_frame_t *message_1) {
  mac_header_t *mac_header = &message_1->mac_header;
  eapol_auth_data_t *auth_data = (eapol_auth_data_t *)message_1->auth_data;
  wpa_key_data_t *key_data = (wpa_key_data_t *)auth_data->wpa_key_data;

  char m1_buffer[90];
  char *p_m1_buffer = m1_buffer;
  put_mac_addr_in_buff(p_m1_buffer, mac_header->receiver_addr);
  p_m1_buffer += 13;
  put_mac_addr_in_buff(p_m1_buffer, mac_header->bssid);
  p_m1_buffer += 13;
  put_key_in_buff(p_m1_buffer, key_data->pmkid);
  printf("{PMKID,%s}\n", m1_buffer);
}

void output_anonce_from_message_1(eapol_frame_t *message_1) {
  mac_header_t *mac_header_message_1 = &message_1->mac_header;
  eapol_auth_data_t *auth_message_1 = (eapol_auth_data_t *)message_1->auth_data;

  char m1_buffer[120];
  put_mac_addr_in_buff(m1_buffer, mac_header_message_1->bssid);
  put_mac_addr_in_buff(m1_buffer, mac_header_message_1->transmitter_addr);
  // * Anonce (Access point nonce)
  put_nonce_in_buff(m1_buffer, auth_message_1->wpa_key_nonce);
  printf("{MIC_MSG_1,%s}\n", m1_buffer);
}

void output_mic_from_message_2(eapol_frame_t *message_2) {
  mac_header_t *mac_header_message_2 = &message_2->mac_header;
  eapol_auth_data_t *auth_message_2 = (eapol_auth_data_t *)message_2->auth_data;
  uint16_t key_data_length = ntohs(auth_message_2->wpa_key_data_length);

  char m2_buffer[250 + (key_data_length + 1)];
  put_mac_addr_in_buff(m2_buffer, mac_header_message_2->bssid);
  put_mac_addr_in_buff(m2_buffer + 18, mac_header_message_2->bssid);
  put_m2_info_in_buff(m2_buffer + 36, auth_message_2);
  // * Snonce (Station nonce)
  put_nonce_in_buff(m2_buffer + 54, auth_message_2->wpa_key_nonce);
  // * MIC (Message Integrity Check)
  put_key_in_buff(m2_buffer + 108, auth_message_2->wpa_key_mic);
  // * Key data
  put_key_data_in_buff(m2_buffer + 216, auth_message_2->wpa_key_data,
                       key_data_length);
  printf("{MIC_MSG_2,%s}\n", m2_buffer);
}