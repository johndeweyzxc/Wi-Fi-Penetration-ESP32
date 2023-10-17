/*
 * @file armament.c
 * @author johndeweyzxc (johndewey02003@gmail.com)
 * @brief Implements functionality for orchestrating wifi attacks
 */

#include "armament.h"

#include <stdio.h>
#include <string.h>

#include "arma_deauth.h"
#include "arma_mic.h"
#include "arma_pmkid.h"
#include "arma_reconnaissance.h"
#include "arma_utils.h"
#include "esp_event.h"
#include "wifi_ctl_interface.h"

ESP_EVENT_DEFINE_BASE(ARMAMENT_CMD_EVENT_BASE);

void armament_activate(armament_cmd_event_data *cmd_event_data) {
  char *arma_selected = cmd_event_data->arma_selected;
  char *target_bssid = cmd_event_data->target_bssid;

  if (memcmp(arma_selected, RECONNAISSANCE, 2) == 0) {
    arma_reconnaissance();
  } else if (memcmp(arma_selected, PMKID, 2) == 0) {
    arma_pmkid(target_bssid);
  } else if (memcmp(arma_selected, MIC, 2) == 0) {
    arma_mic(target_bssid);
  } else if (memcmp(arma_selected, DEAUTH, 2) == 0) {
    arma_deauth(target_bssid);
  }
}

void armament_deactivate(armament_cmd_event_data *cmd_event_data) {
  char *arma_selected = cmd_event_data->arma_selected;

  if (memcmp(arma_selected, PMKID, 2) == 0) {
    arma_pmkid_finishing_sequence(0);
  } else if (memcmp(arma_selected, MIC, 2) == 0) {
    arma_mic_finishing_sequence(0);
  } else if (memcmp(arma_selected, DEAUTH, 2) == 0) {
    arma_deauth_finish();
  }
}

void armament_activate_or_deactivate(void *args, esp_event_base_t event_base,
                                     int32_t event_id, void *event_data) {
  armament_cmd_event_data *cmd_event_data;
  cmd_event_data = (armament_cmd_event_data *)event_data;
  uint8_t activate_arma = cmd_event_data->armament_activate;

  if (activate_arma == 1) {
    armament_activate(cmd_event_data);
  } else if (activate_arma == 0) {
    armament_deactivate(cmd_event_data);
  }
}

void arma_cmd_event_register() {
  wifi_app_init();
  ESP_ERROR_CHECK(esp_event_handler_register(ARMAMENT_CMD_EVENT_BASE, CMD_EVENT,
                                             &armament_activate_or_deactivate,
                                             NULL));
  printf(
      "armament.arma_cmd_event_register > Armament command receiver "
      "registered\n");
}
