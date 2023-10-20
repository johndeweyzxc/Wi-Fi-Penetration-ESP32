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
#include "arma_output.h"
#include "arma_pmkid.h"
#include "arma_reconnaissance.h"
#include "arma_utils.h"
#include "esp_event.h"
#include "esp_system.h"
#include "wifi_ctl_interface.h"

ESP_EVENT_DEFINE_BASE(ARMAMENT_CMD_EVENT_BASE);

void armament_activate(armament_cmd_event_data *cmd_event_data) {
  char *arma_selected = cmd_event_data->arma_selected;
  char *target_bssid = cmd_event_data->target_bssid;

  if (memcmp(arma_selected, RECONNAISSANCE, 2) == 0) {
    printf(
        "armament.armament_activate > Activating RECONNAISSANCE for scanning "
        "access points\n");
    arma_reconnaissance();
  } else if (memcmp(arma_selected, PMKID, 2) == 0) {
    printf("armament.armament_activate > Activating PMKID based attack\n");
    arma_pmkid(target_bssid);
  } else if (memcmp(arma_selected, MIC, 2) == 0) {
    printf("armament.armament_activate > Activating MIC based attack\n");
    arma_mic(target_bssid);
  } else if (memcmp(arma_selected, DEAUTH, 2) == 0) {
    printf("armament.armament_activate > Activating DEAUTH based attack\n");
    arma_deauth(target_bssid);
  }
}

void armament_deactivate(armament_cmd_event_data *cmd_event_data) {
  char *arma_selected = cmd_event_data->arma_selected;
  char *target_bssid = cmd_event_data->target_bssid;
  uint8_t u_target_bssid[6];

  for (uint8_t i = 0; i < 6; i++) {
    uint8_t s1 = target_bssid[i + i];
    uint8_t s2 = target_bssid[i + i + 1];
    u_target_bssid[i] = convert_to_uint8_t(s1, s2);
  }

  if (memcmp(arma_selected, PMKID, 2) == 0) {
    printf("armament.armament_deactivate > Deactivating PMKID\n");
    output_failed_pmkid_attack(u_target_bssid);
    esp_restart();
  } else if (memcmp(arma_selected, MIC, 2) == 0) {
    printf("armament.armament_deactivate > Deactivating MIC\n");
    output_failed_mic_attack(u_target_bssid);
    esp_restart();
  } else if (memcmp(arma_selected, DEAUTH, 2) == 0) {
    printf("armament.armament_deactivate > Deactivating DEAUTH\n");
    arma_deauth_finish();
    output_stopped_deauth(u_target_bssid);
  }
}

void armament_activate_or_deactivate(void *args, esp_event_base_t event_base,
                                     int32_t event_id, void *event_data) {
  armament_cmd_event_data *cmd_event_data;
  cmd_event_data = (armament_cmd_event_data *)event_data;
  uint8_t activate_arma = cmd_event_data->armament_activate;

  if (activate_arma == 1) {
    output_armament_activation();
    armament_activate(cmd_event_data);
  } else if (activate_arma == 0) {
    output_armament_deactivation();
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
