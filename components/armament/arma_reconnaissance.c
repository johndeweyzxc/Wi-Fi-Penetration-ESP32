/**
 * @file arma_reconnaissance.c
 * @author johndeweyzxc (johndewey02003@gmail.com)
 * @brief Declaration of methods for scanning nearby access points
 */

#include "arma_reconnaissance.h"

#include <stdio.h>

#include "arma_utils.h"
#include "esp_wifi_types.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "wifi_ctl_interface.h"

void arma_reconnaissance() {
  wifi_scan_aps();
  ap_list_from_scan_t *ap_list = wifi_get_scanned_aps();
  wifi_ap_record_t *ap_records = ap_list->ap_record_list;
  uint8_t total_scanned_aps = ap_list->count;

  printf("{RECONNAISSANCE,FOUND_APS,%u,}\n", total_scanned_aps);

  for (uint8_t i = 0; i < total_scanned_aps; i++) {
    wifi_ap_record_t ap_record = ap_records[i];
    uint8_t *bssid = ap_record.bssid;

    vTaskDelay(100 / portTICK_PERIOD_MS);

    printf("{RECONNAISSANCE,SCAN,%02X%02X%02X%02X%02X%02X,", bssid[0], bssid[1],
           bssid[2], bssid[3], bssid[4], bssid[5]);
    print_string_into_hex(ap_record.ssid);
    printf("%d,%u,}\n", ap_record.rssi, ap_record.primary);
  }
  printf("{RECONNAISSANCE,FINISH_SCAN,}\n");
}