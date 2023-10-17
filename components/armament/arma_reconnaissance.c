/*
 * @file arma_reconnaissance.c
 * @author johndeweyzxc (johndewey02003@gmail.com)
 * @brief Declaration of methods for scanning nearby access points
 */

#include "arma_reconnaissance.h"

#include <stdio.h>

#include "arma_output.h"
#include "esp_wifi_types.h"
#include "wifi_ctl_interface.h"

void arma_reconnaissance() {
  wifi_scan_aps();
  ap_list_from_scan_t *ap_list = wifi_get_scanned_aps();
  wifi_ap_record_t *ap_records = ap_list->ap_record_list;
  uint16_t total_scanned_aps = ap_list->count;
  output_number_of_aps(total_scanned_aps);

  for (uint16_t i = 0; i < total_scanned_aps; i++) {
    wifi_ap_record_t ap_record = ap_records[i];
    output_ap_info(&ap_record);
  }
}