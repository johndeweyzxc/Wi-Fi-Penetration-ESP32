/*
 * @file arma_utils.c
 * @author johndeweyzxc (johndewey02003@gmail.com)
 * @brief Implements functionality for utils such as formatting outputs
 */

#include "arma_utils.h"

#include <stdio.h>

#include "esp_wifi_types.h"

void output_ap_info(wifi_ap_record_t *ap_record) {
  uint8_t *bssid = ap_record->bssid;
  char output_buffer[100];
  char *p_output_buffer = output_buffer;

  sprintf(p_output_buffer, "%02X%02X%02X%02X%02X%02X,", bssid[0], bssid[1],
          bssid[2], bssid[3], bssid[4], bssid[5]);

  p_output_buffer += 18;
  for (uint8_t i = 0; i < 33; i++) {
    if (ap_record->ssid[i] == 0) {
      break;
    } else {
      sprintf(p_output_buffer, "%02X", ap_record->ssid[i]);
      p_output_buffer += 2;
    }
  }
  sprintf(p_output_buffer, ",%02X", (uint8_t)ap_record->rssi);
  p_output_buffer += 3;
  sprintf(p_output_buffer, ",%02X,", (uint8_t)ap_record->primary);

  printf("{SCAN,%s}\n", output_buffer);
}