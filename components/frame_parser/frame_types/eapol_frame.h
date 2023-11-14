/**
 * @file eapol_frame.h
 * @author johndeweyzxc (johndewey02003@gmail.com)
 * @brief Declaration of EAPOL data frame structure where key vital information
 * in wifi penetration is found, the PMKID and MIC.
 */

#pragma once

#include <stdint.h>

#include "frame_constants.h"

/**
 * @brief WPA PMKID key data (22 bytes)
 * See ref: 802.11-2016 12.7.2
 */
typedef struct {
  uint8_t key_data_type;
  uint8_t length;
  uint8_t oui[3];
  uint8_t oui_type;
  uint8_t pmkid[16];
} wpa_key_data_t;

/**
 * @brief Key information data (2 bytes)
 * See ref: 802.11-2016 12.7.2
 */
typedef struct {
  uint8_t reserved : 2;
  uint8_t smk_message : 1;
  uint8_t encrypted_key_data : 1;
  uint8_t request : 1;
  uint8_t error : 1;
  uint8_t secure : 1;
  uint8_t key_mic : 1;
  uint8_t key_ack : 1;
  uint8_t install : 1;
  uint8_t key_index : 2;
  uint8_t key_type : 1;
  uint8_t key_descriptor_version : 3;
} key_information_t;

/**
 * @brief Authentication data. TODO: The length of MIC is dependent on the
 * negotiated AKM, see ref: 802.11-2016 12.7.3
 *
 * See ref: 802.11-2016 12.7.2
 */
typedef struct __attribute__((__packed__)) {
  uint8_t version;
  uint8_t type;
  uint8_t length[2];
  uint8_t key_desc_type;
  uint16_t key_info;
  uint16_t key_length;
  uint8_t replay_counter[8];
  uint8_t wpa_key_nonce[32];
  uint8_t wpa_key_iv[16];
  uint8_t wpa_key_rsc[8];
  uint8_t wpa_key_id[8];
  uint8_t wpa_key_mic[16];
  uint16_t wpa_key_data_length;
  uint8_t wpa_key_data[];
} eapol_auth_data_t;

/**
 * @brief: Logical Link Control (8 bytes)
 */
typedef struct {
  uint8_t dsap_snap;
  uint8_t ssap_snap;
  uint8_t control_field;
  uint8_t organization_code[3];
  uint16_t authentication_type;
} logical_link_control_t;

/**
 * @brief Structure of the eapol frame
 */
typedef struct {
  frame_control_t frame_control;
  uint16_t duration;
  mac_header_t mac_header;
  uint16_t sequence_and_fragment_number;
  uint16_t qos_control;
  logical_link_control_t logical_link_control;
  uint8_t auth_data[];
} eapol_frame_t;

#define EAPOL_AUTH_TYPE 0x888e
#define EAPOL_KEY_TYPE 0x03

/**
 * See ref: 802.11-2016 [12.7.2]
 */
#define WPA_KEY_DATA_TYPE 0xdd
/**
 * See ref: 802.11-2016 [12.7.2]
 */
#define WPA_KEY_DATA_TYPE_PMKID_KDE 4

/**
 * See ref: 802.11-2016 [12.7.2, Table 12-6]
 */
static const uint8_t WPA_KEY_DATA_OUI[3] = {0x00, 0x0f, 0xac};