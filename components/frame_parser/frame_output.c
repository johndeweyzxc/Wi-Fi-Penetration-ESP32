/*
 * @file frame_output.c
 * @author johndeweyzxc (johndewey02003@gmail.com)
 * @brief Implementation of methods for formatting and outputting PMKID and MIC
 * from eapol data.
 */

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
  char *p_local_char_buff = char_buff;
  sprintf(p_local_char_buff,
          "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
          n[0], n[1], n[2], n[3], n[4], n[5], n[6], n[7], n[8], n[9], n[10],
          n[11], n[12], n[13], n[14], n[15]);
  p_local_char_buff += 32;
  sprintf(p_local_char_buff,
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
  uint8_t key_info_b[2];
  uint8_t key_len_b[2];

  key_info_b[0] = (uint8_t)(auth_message_2->key_info & 0xFF);
  key_info_b[1] = (uint8_t)((auth_message_2->key_info >> 8) & 0xFF);

  key_len_b[0] = (uint8_t)(auth_message_2->key_length & 0xFF);
  key_len_b[1] = (uint8_t)((auth_message_2->key_length >> 8) & 0xFF);

  sprintf(char_buff, "%02X,%02X,%02X%02X,%02X,%02X%02X,%02X%02X,",
          auth_message_2->version, auth_message_2->type,
          auth_message_2->length[0], auth_message_2->length[1],
          auth_message_2->key_desc_type, key_info_b[0], key_info_b[1],
          key_len_b[0], key_len_b[1]);
}

void put_replay_counter_in_buff(char *char_buff, uint8_t *rc) {
  sprintf(char_buff, "%02X%02X%02X%02X%02X%02X%02X%02X,", rc[0], rc[1], rc[2],
          rc[3], rc[4], rc[5], rc[6], rc[7]);
}

void put_key_iv_in_buff(char *char_buff, uint8_t *iv) {
  sprintf(char_buff,
          "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X,",
          iv[0], iv[1], iv[2], iv[3], iv[4], iv[5], iv[6], iv[7], iv[8], iv[9],
          iv[10], iv[11], iv[12], iv[13], iv[14], iv[15]);
}

void put_id_or_rsc_in_buff(char *char_buff, uint8_t *b) {
  sprintf(char_buff, "%02X%02X%02X%02X%02X%02X%02X%02X,", b[0], b[1], b[2],
          b[3], b[4], b[5], b[6], b[7]);
}

void put_key_data_length_in_buff(char *char_buff,
                                 eapol_auth_data_t *auth_message_2) {
  uint8_t key_data_len_b[2];

  key_data_len_b[0] = (uint8_t)(auth_message_2->wpa_key_data_length & 0xFF);
  key_data_len_b[1] =
      (uint8_t)((auth_message_2->wpa_key_data_length >> 8) & 0xFF);

  sprintf(char_buff, "%02X%02X,", key_data_len_b[0], key_data_len_b[1]);
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

  char m1_buffer[50];
  char *p_m1_buffer = m1_buffer;

  // Transmitter address (Access point mac address)
  put_mac_addr_in_buff(p_m1_buffer, mac_header->transmitter_addr);
  p_m1_buffer += 13;
  // Receiver address (Station mac address)
  put_mac_addr_in_buff(p_m1_buffer, mac_header->receiver_addr);
  p_m1_buffer += 13;
  // PMKID (Pairwise Master Key Identifier)
  put_key_in_buff(p_m1_buffer, key_data->pmkid);
  printf("{PMKID,MSG_1,%s}\n", m1_buffer);
}

void output_anonce_from_message_1(eapol_frame_t *message_1) {
  mac_header_t *mac_header_message_1 = &message_1->mac_header;
  eapol_auth_data_t *auth_message_1 = (eapol_auth_data_t *)message_1->auth_data;

  char m1_buffer[91 + 50];
  char *p_m1_buffer = m1_buffer;

  // Transmitter address (Access point mac address)
  put_mac_addr_in_buff(p_m1_buffer, mac_header_message_1->transmitter_addr);
  p_m1_buffer += 13;
  // Receiver address (Station mac address)
  put_mac_addr_in_buff(p_m1_buffer, mac_header_message_1->receiver_addr);
  p_m1_buffer += 13;
  // Anonce (Access point nonce)
  put_nonce_in_buff(p_m1_buffer, auth_message_1->wpa_key_nonce);
  printf("{MIC,MSG_1,%s}\n", m1_buffer);
}

void output_mic_from_message_2(eapol_frame_t *message_2) {
  mac_header_t *mac_header_message_2 = &message_2->mac_header;
  eapol_auth_data_t *auth_message_2 = (eapol_auth_data_t *)message_2->auth_data;
  uint16_t key_data_length = ntohs(auth_message_2->wpa_key_data_length);

  char m2_buffer[234 + (key_data_length + 1) + 50];
  char *p_m2_buffer = m2_buffer;

  // Transmitter address (Station mac address)
  put_mac_addr_in_buff(p_m2_buffer, mac_header_message_2->transmitter_addr);
  p_m2_buffer += 13;
  // Receiver address (Access point mac address)
  put_mac_addr_in_buff(p_m2_buffer, mac_header_message_2->receiver_addr);
  p_m2_buffer += 13;
  // Version, Type, Length, Key Description Type, Key Information and Key Length
  put_m2_info_in_buff(p_m2_buffer, auth_message_2);
  p_m2_buffer += 24;
  // Replay counter
  put_replay_counter_in_buff(p_m2_buffer, auth_message_2->replay_counter);
  p_m2_buffer += 17;
  // WPA Key Nonce (Snonce or Station nonce)
  put_nonce_in_buff(p_m2_buffer, auth_message_2->wpa_key_nonce);
  p_m2_buffer += 65;
  // WPA Key IV
  put_key_iv_in_buff(p_m2_buffer, auth_message_2->wpa_key_iv);
  p_m2_buffer += 33;
  // WPA Key RSC
  put_id_or_rsc_in_buff(p_m2_buffer, auth_message_2->wpa_key_rsc);
  p_m2_buffer += 17;
  // WPA Key ID
  put_id_or_rsc_in_buff(p_m2_buffer, auth_message_2->wpa_key_id);
  p_m2_buffer += 17;
  // MIC (Message Integrity Check)
  put_key_in_buff(p_m2_buffer, auth_message_2->wpa_key_mic);
  p_m2_buffer += 33;
  // WPA Key Data Length
  put_key_data_length_in_buff(p_m2_buffer, auth_message_2);
  p_m2_buffer += 5;
  // WPA Key data
  put_key_data_in_buff(p_m2_buffer, auth_message_2->wpa_key_data,
                       key_data_length);
  p_m2_buffer += (key_data_length * 2);

  sprintf(p_m2_buffer, ",");
  printf("{MIC,MSG_2,%s}\n", m2_buffer);
}