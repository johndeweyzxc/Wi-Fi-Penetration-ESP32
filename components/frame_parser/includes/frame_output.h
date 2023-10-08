#pragma once

#include <stdint.h>

#include "eapol_frame.h"

void put_mac_addr_in_buff(char *char_buff, uint8_t *addr);

void put_nonce_in_buff(char *char_buff, uint8_t *n);

void put_key_in_buff(char *char_buff, uint8_t *k);

void put_m2_info_in_buff(char *char_buff, eapol_auth_data_t *auth_message_2);

void put_key_data_in_buff(char *char_buff, uint8_t *key_data, uint16_t length);

void output_pmkid(eapol_frame_t *message_1);

void output_anonce_from_message_1(eapol_frame_t *message_1);

void output_mic_from_message_2(eapol_frame_t *message_2);