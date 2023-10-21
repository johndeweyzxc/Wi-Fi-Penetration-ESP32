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
  printf("{MIC_DEAUTH_STARTED,%02X%02X%02X%02X%02X%02X,}\n", b[0], b[1], b[2],
         b[3], b[4], b[5]);
}

void output_mic_launching_sequence() { printf("{MIC_LAUNCHING_SEQUENCE,}\n"); }

void output_mic_deauth_status(int current_time) {
  printf("{MIC_DEAUTH_TIME_ELAPSED,%d,}\n", current_time);
}

void output_failed_mic_not_found_ap(uint8_t *target_bssid) {
  uint8_t *b = target_bssid;
  printf("{MIC_AP_NOT_FOUND,%02X%02X%02X%02X%02X%02X,}\n", b[0], b[1], b[2],
         b[3], b[4], b[5]);
}

void output_mic_finishing_sequence() { printf("{MIC_FINISHING_SEQUENCE,}\n"); }

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
  printf("{PMKID_SNIFF_STARTED,%02X%02X%02X%02X%02X%02X,}\n", b[0], b[1], b[2],
         b[3], b[4], b[5]);
}

void output_failed_pmkid_not_found_ap(uint8_t *target_bssid) {
  uint8_t *b = target_bssid;
  printf("{PMKID_AP_NOT_FOUND,%02X%02X%02X%02X%02X%02X,}\n", b[0], b[1], b[2],
         b[3], b[4], b[5]);
}

void output_success_pmkid_attack(uint8_t *target_bssid) {
  uint8_t *b = target_bssid;
  printf("{PMKID_SUCCESS,%02X%02X%02X%02X%02X%02X,}\n", b[0], b[1], b[2], b[3],
         b[4], b[5]);
}

void output_pmkid_launching_sequence() {
  printf("{PMKID_LAUNCHING_SEQUENCE,}\n");
}

void output_pmkid_sniff_status(int current_time) {
  printf("{PMKID_SNIFF_TIME_ELAPSED,%d,}\n", current_time);
}

void output_pmkid_finishing_sequence() {
  printf("{PMKID_FINISHING_SEQUENCE,}\n");
}

void output_failed_pmkid_attack(uint8_t *target_bssid) {
  uint8_t *b = target_bssid;
  printf("{PMKID_FAILURE,%02X%02X%02X%02X%02X%02X,}\n", b[0], b[1], b[2], b[3],
         b[4], b[5]);
}

void output_deauth_launching_sequence() {
  printf("{DEAUTH_LAUNCHING_SEQUENCE,}\n");
}

void output_started_deauth(uint8_t *target_bssid) {
  uint8_t *b = target_bssid;
  printf("{DEAUTH_STARTED,%02X%02X%02X%02X%02X%02X,}\n", b[0], b[1], b[2], b[3],
         b[4], b[5]);
}

void output_deauth_status(int current_time) {
  printf("{DEAUTH_TIME_ELAPSED,%d,}\n", current_time);
}

void output_failed_deauth_not_found_ap(uint8_t *target_bssid) {
  uint8_t *b = target_bssid;
  printf("{DEAUTH_AP_NOT_FOUND,%02X%02X%02X%02X%02X%02X,}\n", b[0], b[1], b[2],
         b[3], b[4], b[5]);
}

void output_stopped_deauth(uint8_t *target_bssid) {
  uint8_t *b = target_bssid;
  printf("{DEAUTH_STOPPED,%02X%02X%02X%02X%02X%02X,}\n", b[0], b[1], b[2], b[3],
         b[4], b[5]);
}

void output_armament_activation() { printf("{ARMAMENT_ACTIVATE,}\n"); }

void output_armament_deactivation() { printf("{ARMAMENT_DEACTIVATE,}\n"); }