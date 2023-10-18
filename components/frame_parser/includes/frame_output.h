/*
 * @file frame_output.h
 * @author johndeweyzxc (johndewey02003@gmail.com)
 * @brief Declaration of methods for formatting and outputting PMKID and MIC
 * from eapol data.
 */

#pragma once

#include <stdint.h>

#include "eapol_frame.h"

/*
 * @brief Formats the mac address and copy in buffer
 * @param *char_buff buffer to store the output
 * @param *addr mac address (6 bytes)
 */
void put_mac_addr_in_buff(char *char_buff, uint8_t *addr);

/*
 * @brief Formats the snonce or anonce and copy in buffer using sprintf
 * @param *char_buff buffer to store output
 * @param *n station nonce or access point nonce (32 bytes)
 */
void put_nonce_in_buff(char *char_buff, uint8_t *n);

/*
 * @brief Formats the MIC or PMKID and copy in buffer
 * @param *char_buff buffer to store output
 * @param *n MIC or PMKID (16 bytes)
 */
void put_key_in_buff(char *char_buff, uint8_t *k);

/*
 * @brief Formats the version, type, length and key description type of
 * authentication and copy in buffer
 * @param *char_buff buffer to store output
 * @param *auth_message_2 eapol data of second message from 4 way handshake
 */
void put_m2_info_in_buff(char *char_buff, eapol_auth_data_t *auth_message_2);

/*
 * @brief Formats the replay counter and copy in buffer
 * @param *char_buff buffer to store output
 * @param *rc the replay counter in the eapol message
 */
void put_replay_counter_in_buff(char *char_buff, uint8_t *rc);

/*
 * @brief Formats the key data which is the data after the MIC or PMKID and copy
 * in buffer
 * @param *char_buff buffer to store output
 * @param *key_data the data after the MIC or PMKID in the eapol message
 * @param length length of key_data
 */
void put_key_data_in_buff(char *char_buff, uint8_t *key_data, uint16_t length);

/*
 * @brief Puts the data in the output buffer *char_buff and prints the data
 * @param *message_1 eapol data of first message from 4 way handshake
 */
void output_pmkid(eapol_frame_t *message_1);

/*
 * @brief Puts the bssid, transmitter address and access point nonce in the
 * output buffer and prints the data
 * @param *message_1 eapol data of first message from 4 way handshake
 */
void output_anonce_from_message_1(eapol_frame_t *message_1);

/*
 * @brief Puts the bssid, transmitter addres, station nonce, mic, and key data
 * in the output buffer *char_buff and prints the data
 * @param *message_2 eapol data of the second message from 4 way handshake
 */
void output_mic_from_message_2(eapol_frame_t *message_2);