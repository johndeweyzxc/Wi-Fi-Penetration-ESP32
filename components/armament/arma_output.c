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