/*
 * @file arma_mic.h
 * @author johndeweyzxc (johndewey02003@gmail.com)
 * @brief Implementation of methods for outputting formatted data to serial
 */

#include "arma_output.h"

#include <stdio.h>

#include "esp_wifi_types.h"

void output_ap_info(wifi_ap_record_t *ap_record) {
  uint8_t *bssid = ap_record->bssid;
  printf("{SCAN,%02X%02X%02X%02X%02X%02X,%s,%02X,%02X,}\n", bssid[0], bssid[1],
         bssid[2], bssid[3], bssid[4], bssid[5], ap_record->ssid,
         (uint8_t)ap_record->rssi, (uint8_t)ap_record->primary);
}

void output_number_of_aps(uint16_t num) { printf("{FOUND_APS,%u,}\n", num); }

void output_mic_operation_started(uint8_t *target_bssid) {
  uint8_t *b = target_bssid;
  printf("{MIC_OPERATION_STARTED,%02X%02X%02X%02X%02X%02X,}\n", b[0], b[1],
         b[2], b[3], b[4], b[5]);
}

void output_success_mic_attack(uint8_t *target_bssid) {
  uint8_t *b = target_bssid;
  printf("{MIC_SUCCESS,%02X%02X%02X%02X%02X%02X,}\n", b[0], b[1], b[2], b[3],
         b[4], b[5]);
}

void output_failed_mic_attack(uint8_t *target_bssid) {
  uint8_t *b = target_bssid;
  printf("{MIC_FAILURE,%02X%02X%02X%02X%02X%02X,}\n", b[0], b[1], b[2], b[3],
         b[4], b[5]);
}

void output_pmkid_operation_started(uint8_t *target_bssid) {
  uint8_t *b = target_bssid;
  printf("{PMKID_OPERATION_STARTED,%02X%02X%02X%02X%02X%02X,}\n", b[0], b[1],
         b[2], b[3], b[4], b[5]);
}

void output_failed_pmkid_not_found_ap(uint8_t *target_bssid) {
  uint8_t *b = target_bssid;
  printf("{PMKID_OPERATION_NOT_FOUND_AP,%02X%02X%02X%02X%02X%02X,}\n", b[0],
         b[1], b[2], b[3], b[4], b[5]);
}

void output_success_pmkid_attack(uint8_t *target_bssid) {
  uint8_t *b = target_bssid;
  printf("{PMKID_SUCCESS,%02X%02X%02X%02X%02X%02X,}\n", b[0], b[1], b[2], b[3],
         b[4], b[5]);
}

void output_failed_pmkid_attack(uint8_t *target_bssid) {
  uint8_t *b = target_bssid;
  printf("{PMKID_FAILURE,%02X%02X%02X%02X%02X%02X,}\n", b[0], b[1], b[2], b[3],
         b[4], b[5]);
}

void output_started_deauth(uint8_t *target_bssid) {
  uint8_t *b = target_bssid;
  printf("{DEAUTH_STARTED,%02X%02X%02X%02X%02X%02X,}\n", b[0], b[1], b[2], b[3],
         b[4], b[5]);
}

void output_failed_deauth_not_found_ap(uint8_t *target_bssid) {
  uint8_t *b = target_bssid;
  printf("{DEAUTH_OPERATION_NOT_FOUND_AP,%02X%02X%02X%02X%02X%02X,}\n", b[0],
         b[1], b[2], b[3], b[4], b[5]);
}

void output_stopped_deauth(uint8_t *target_bssid) {
  uint8_t *b = target_bssid;
  printf("{DEAUTH_STOPPED,%02X%02X%02X%02X%02X%02X,}\n", b[0], b[1], b[2], b[3],
         b[4], b[5]);
}
