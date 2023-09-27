#include "armament.h"

#include <stdio.h>
#include <string.h>

#include "esp_event.h"
#include "esp_log.h"
#include "esp_wifi_types.h"
#include "frame_parser_interface.h"
#include "wifi_ctl_interface.h"

ESP_EVENT_DEFINE_BASE(ARMAMENT_CMD_EVENT_BASE);
static const char *TAG = "ARMAMENT";
static uint8_t wifi_init_status = WIFI_NOT_INITIALIZED;

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
  sprintf(p_output_buffer, ",%02X", (uint8_t)ap_record->primary);

  printf("{%s}\n", output_buffer);
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

static void armament_activate(void *args, esp_event_base_t event_base,
                              int32_t event_id, void *event_data) {
  armament_cmd_event_data *cmd_event_data;
  cmd_event_data = (armament_cmd_event_data *)event_data;

  char *arma_selected = cmd_event_data->arma_selected;

  if (wifi_init_status == WIFI_NOT_INITIALIZED) {
    wifi_app_init();
    wifi_init_status = WIFI_INITIALIZED;
  }

  printf("%02X%02X\n", arma_selected[0], arma_selected[1]);

  if (memcmp(arma_selected, RECONNAISSANCE, 2) == 0) {
    ESP_LOGI(TAG, "Using reconnaissance...");
    arma_reconnaissance();
  } else if (memcmp(arma_selected, PMKID, 2) == 0) {
    // TODO: Attack method is pmkid
  } else if (memcmp(arma_selected, MIC, 2) == 0) {
    // TODO: Attack method is mic
  } else if (memcmp(arma_selected, DEAUTH, 2) == 0) {
    // TODO: Deauth a client
  }
}

void armament_cmd_event_register() {
  ESP_ERROR_CHECK(esp_event_handler_register(ARMAMENT_CMD_EVENT_BASE, CMD_EVENT,
                                             &armament_activate, NULL));
  ESP_LOGI(TAG, "Command event handler registered...");
}

void armament_cmd_event_unregister() {
  ESP_ERROR_CHECK(esp_event_handler_unregister(ARMAMENT_CMD_EVENT_BASE,
                                               CMD_EVENT, &armament_activate));
  ESP_LOGI(TAG, "Command event handler unregistered...");
}
