#pragma once

#include <stdint.h>

#include "eapol_frame.h"

void put_mac_addr_in_buff(char *char_buff, uint8_t *addr);

void put_nonce_in_buff(char *char_buff, uint8_t *n);

void output_pmkid(eapol_frame_t *message_1);

void output_mic_message_1(eapol_frame_t *message_1);

void output_mic_message_2(eapol_frame_t *message_2);