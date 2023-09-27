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

void store_key_in_buff(char *char_buff, uint8_t *k) {
  sprintf(char_buff,
          "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X,",
          k[0], k[1], k[2], k[3], k[4], k[5], k[6], k[7], k[8], k[9], k[10],
          k[11], k[12], k[13], k[14], k[15]);
}

void ouput_pmkid(eapol_frame_t *message_1) {
  mac_header_t *mac_header = &message_1->mac_header;
  eapol_auth_data_t *auth_data = (eapol_auth_data_t *)message_1->auth_data;
  wpa_key_data_t *key_data = (wpa_key_data_t *)auth_data->wpa_key_data;

  char m1_buffer[28];
  put_mac_addr_in_buff(m1_buffer, mac_header->bssid);
  put_mac_addr_in_buff(m1_buffer + 6, mac_header->receiver_addr);
  store_key_in_buff(m1_buffer + 12, key_data->pmkid);
  printf("{%s}\n", m1_buffer);
}

void output_mic_message_1(eapol_frame_t *message_1) {
  // TODO: Finish this
}

void output_mic_message_2(eapol_frame_t *message_2) {
  mac_header_t *mac_header_message_2 = &message_2->mac_header;
  eapol_auth_data_t *auth_message_2 = (eapol_auth_data_t *)message_2->auth_data;
  uint16_t key_data_length = ntohs(auth_message_2->wpa_key_data_length);

  char m2_buffer[64 + key_data_length];
  put_mac_addr_in_buff(m2_buffer, mac_header_message_2->bssid);
  put_mac_addr_in_buff(m2_buffer + 6, mac_header_message_2->bssid);

  char *p_m2_buffer = m2_buffer;
  p_m2_buffer += 12;
  sprintf(p_m2_buffer, "%02X,%02X,%02X%02X,%02X,", auth_message_2->version,
          auth_message_2->type, auth_message_2->length[0],
          auth_message_2->length[1], auth_message_2->key_desc_type);

  put_nonce_in_buff(m2_buffer, auth_message_2->wpa_key_nonce);
  // TODO: Finish this
}