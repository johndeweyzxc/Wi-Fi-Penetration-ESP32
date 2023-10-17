/*
 * @file frame_eapol_validator.h
 * @author johndeweyzxc (johndewey02003@gmail.com)
 * @brief Declaration of methods for validating eapol data
 */

#pragma once
#include <stdint.h>

#include "eapol_frame.h"
#include "frame_constants.h"

/*
 * @brief enum return by eapol_valid_pmkid() which returns either WRONG_PMKID or
 * GOOD_PMKID
 */
enum return_status_valid_pmkid { WRONG_PMKID, GOOD_PMKID };

/*
 * @brief enum return by is_eapol_auth_type() which returns either
 * WRONG_EAPOL_AUTH_TYPE or GOOD_EAPOL_AUTH_TYPE
 */
enum return_status_eapol_auth_type {
  WRONG_EAPOL_AUTH_TYPE,
  GOOD_EAPOL_AUTH_TYPE
};

/*
 * @brief enum return by bssid_in_eapol_matched() which returns either
 * BSSID_NOT_MATCHED or BSSID_MATCHED
 */
enum return_status_eapol_matched { BSSID_NOT_MATCHED, BSSID_MATCHED };

/*
 * @brief enum return by is_qos_data() which returns either NOT_QOS or IS_QOS
 */
enum return_status_qos_data { NOT_QOS, IS_QOS };

/*
 * @brief Checks if it is a valid PMKID data by checking key data type, oui,
 * and oui type
 * @param *key_data the wpa key data from the first message of 4 way handshake
 */
uint8_t eapol_valid_pmkid(wpa_key_data_t *key_data);

/*
 * @brief Checks if the eapol type in Logical Link Control is Authentication
 * (0x888e)
 * @param *eapol_frame eapol frame from first or second message of 4 way
 * handshake
 */
uint8_t is_eapol_auth_type(eapol_frame_t *eapol_frame);

/*
 * @brief Checks if the bssid in eapol frame matches with the set target_bssid
 * @param *eapol_frame eapol frame from first or second message of 4 way
 * @param *target_bssid mac address of the target (6 bytes)
 */
uint8_t bssid_in_eapol_matched(eapol_frame_t *eapol_frame,
                               uint8_t *target_bssid);

/*
 * @brief Checks if data frame is a QoS subtype by checking subtype in frame
 * control
 * @param *frame_control frame control in eapol frame
 */
uint8_t is_qos_data(frame_control_t *frame_control);
