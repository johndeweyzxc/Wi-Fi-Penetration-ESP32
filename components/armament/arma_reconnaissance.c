/*
 * @file arma_reconnaissance.c
 * @author johndeweyzxc (johndewey02003@gmail.com)
 * @brief Declaration of methods for scanning nearby access points
 */

#include "arma_reconnaissance.h"

#include <stdio.h>

#include "esp_wifi_types.h"
#include "wifi_ctl_interface.h"

void arma_reconnaissance() {
  wifi_scan_aps();
  ap_list_from_scan_t *ap_list = wifi_get_scanned_aps();
  wifi_ap_record_t *ap_records = ap_list->ap_record_list;
  uint16_t total_scanned_aps = ap_list->count;
  printf("{RECONNAISSANCE,FOUND_APS,%u,}\n", total_scanned_aps);

  for (uint16_t i = 0; i < total_scanned_aps; i++) {
    wifi_ap_record_t ap_record = ap_records[i];
    uint8_t *bssid = ap_record.bssid;
    // An RSSI of -124 is 132 if typecasted from int to uint to bring back
    // original value do: 255 - 132 + 1 = 124.
    printf("{RECONNAISSANCE,SCAN,%02X%02X%02X%02X%02X%02X,%s,%d,%u,}\n",
           bssid[0], bssid[1], bssid[2], bssid[3], bssid[4], bssid[5],
           ap_record.ssid, ap_record.rssi, ap_record.primary);
  }
  printf("{RECONNAISSANCE,FINISH_SCAN}\n");
}