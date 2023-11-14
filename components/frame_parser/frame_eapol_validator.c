/**
 * @file frame_eapol_validator.c
 * @author johndeweyzxc (johndewey02003@gmail.com)
 * @brief Implements functionality for validating eapol data from 4 way
 * handshake
 */

#include "frame_eapol_validator.h"

#include <stdio.h>

#include "arpa/inet.h"
#include "eapol_frame.h"
#include "frame_constants.h"

uint8_t eapol_valid_pmkid(wpa_key_data_t *key_data) {
  uint8_t data_type = key_data->key_data_type;
  uint8_t *oui = key_data->oui;
  uint8_t oui_type = key_data->oui_type;

  if (data_type != WPA_KEY_DATA_TYPE) {
    printf("{PMKID,WRONG_KEY_TYPE,%02X,}\n", data_type);
    return WRONG_PMKID;
  }

  for (uint8_t i = 0; i < 3; i++) {
    if (oui[i] != WPA_KEY_DATA_OUI[i]) {
      printf("{PMKID,WRONG_OUI,%02X%02X%02X,}\n", oui[0], oui[1], oui[2]);
      return WRONG_PMKID;
    }
  }

  if (oui_type != WPA_KEY_DATA_TYPE_PMKID_KDE) {
    printf("{PMKID,WRONG_KDE,%02X,}\n", oui_type);
    return WRONG_PMKID;
  }
  return GOOD_PMKID;
}

uint8_t is_eapol_auth_type(eapol_frame_t *eapol_frame) {
  logical_link_control_t *logic_link_ctrl = &eapol_frame->logical_link_control;
  uint16_t auth_type = logic_link_ctrl->authentication_type;

  if (ntohs(auth_type) != EAPOL_AUTH_TYPE) {
    printf("eapol_validator.is_eapol_auth_type > Wrong eapol auth type: %02X\n",
           auth_type);
    return WRONG_EAPOL_AUTH_TYPE;
  } else {
    return GOOD_EAPOL_AUTH_TYPE;
  }
}

uint8_t bssid_in_eapol_matched(eapol_frame_t *eapol_frame,
                               uint8_t *target_bssid) {
  mac_header_t *mac_header = &eapol_frame->mac_header;
  uint8_t *bssid_in_eapol = mac_header->bssid;

  for (uint8_t i = 0; i < 6; i++) {
    if (bssid_in_eapol[i] != target_bssid[i]) {
      return BSSID_NOT_MATCHED;
    }
  }

  return BSSID_MATCHED;
}

uint8_t is_qos_data(frame_control_t *frame_control) {
  if (frame_control->protected_frame == 1) return 0;
  if (frame_control->subtype != 8) {
    return NOT_QOS;
  }
  return IS_QOS;
}