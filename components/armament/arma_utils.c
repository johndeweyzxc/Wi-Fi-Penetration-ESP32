/*
 * @file arma_utils.c
 * @author johndeweyzxc (johndewey02003@gmail.com)
 * @brief Implements functionality for utils such as formatting outputs
 */

#include "arma_utils.h"

#include <stdio.h>
#include <string.h>

#include "esp_wifi_types.h"

char *string_append(char *s1, char *s2) {
  int s1_length = strlen(s1);
  int s2_length = strlen(s2);
  int size = s1_length + s2_length;
  char *s = calloc(size, sizeof(char));

  for (int i = 0; i < s1_length; i++) {
    s[i] = s1[i];
  }
  for (int i = 0; i < s2_length; i++) {
    s[s1_length + i] = s2[i];
  }
  return s;
}

uint8_t convert_to_uint8_t(char s1, char s2) {
  char *s;
  uint8_t uint8_bit;
  if (s1 == '\0' || s2 == '\0') return 0;
  char s1a[] = {s1, '\0'};
  char s2a[] = {s2, '\0'};
  s = string_append(s1a, s2a);
  uint8_bit = strtol(s, NULL, 16);
  free(s);
  return uint8_bit;
}

void output_ap_info(wifi_ap_record_t *ap_record) {
  uint8_t *bssid = ap_record->bssid;
  char output_buffer[100];
  char *p_output_buffer = output_buffer;

  sprintf(p_output_buffer, "%02X%02X%02X%02X%02X%02X,", bssid[0], bssid[1],
          bssid[2], bssid[3], bssid[4], bssid[5]);

  p_output_buffer += 13;
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