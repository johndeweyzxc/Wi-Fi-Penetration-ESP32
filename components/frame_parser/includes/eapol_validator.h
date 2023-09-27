#pragma once
#include <stdint.h>

#include "eapol_frame.h"
#include "frame_constants.h"

enum return_status_valid_pmkid { WRONG_PMKID, GOOD_PMKID };

enum return_status_eapol_auth_type {
  WRONG_EAPOL_AUTH_TYPE,
  GOOD_EAPOL_AUTH_TYPE
};

enum return_status_eapol_matched { BSSID_NOT_MATCHED, BSSID_MATCHED };

enum return_status_qos_data { NOT_QOS, IS_QOS };

uint8_t eapol_has_valid_pmkid_key_data(wpa_key_data_t *key_data);

uint8_t is_eapol_auth_type(eapol_frame_t *eapol_frame);

uint8_t bssid_in_eapol_matched(eapol_frame_t *eapol_frame,
                               uint8_t *target_bssid);

uint8_t is_qos_data(frame_control_t *frame_control);
