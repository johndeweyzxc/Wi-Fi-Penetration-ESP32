/*
 * @file armament.c
 * @author johndeweyzxc (johndewey02003@gmail.com)
 * @brief Implements functionality for orchestrating wifi attacks
 */

#include "armament.h"

#include <stdio.h>
#include <string.h>

#include "arma_pmkid.h"
#include "arma_reconnaissance.h"
#include "arma_utils.h"
#include "esp_event.h"
#include "wifi_ctl_interface.h"

ESP_EVENT_DEFINE_BASE(ARMAMENT_CMD_EVENT_BASE);
static uint8_t wifi_init_status = WIFI_NOT_INITIALIZED;

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
    arma_reconnaissance();
  } else if (memcmp(arma_selected, PMKID, 2) == 0) {
    arma_pmkid(target_bssid);
  } else if (memcmp(arma_selected, MIC, 2) == 0) {
    // TODO: Attack method is mic
  } else if (memcmp(arma_selected, DEAUTH, 2) == 0) {
    // TODO: Deauth a client
  }
}

void arma_cmd_event_register() {
  ESP_ERROR_CHECK(esp_event_handler_register(ARMAMENT_CMD_EVENT_BASE, CMD_EVENT,
                                             &armament_activate, NULL));
  printf("armament.arma_cmd_event_register > Arma cmd handler registered\n");
}
