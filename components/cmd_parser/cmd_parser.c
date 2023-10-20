/*
 * @file cmd_parser.c
 * @author johndeweyzxc (johndewey02003@gmail.com)
 * @brief Implements functionality for parsing the input command
 */

#include "cmd_parser.h"

#include <stdio.h>
#include <string.h>

#include "armament_interface.h"
#include "cmd_output.h"
#include "esp_event.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

ESP_EVENT_DECLARE_BASE(ARMAMENT_CMD_EVENT_BASE);
static TaskHandle_t cmd_parser_task_handle = NULL;

void clear_user_in_buff(char *user_in_buff) {
  for (uint8_t i = 0; i < 14; i++) {
    user_in_buff[i] = '0';
  }
}

void clear_arma_and_bssid_buff(char *arma_selected, char *target_bssid) {
  arma_selected[0] = '0';
  arma_selected[1] = '0';

  for (uint8_t i = 0; i < 12; i++) {
    target_bssid[i] = '0';
  }
}

void set_arma_and_target(char *user_in_buff, char *arma_selected,
                         char *target_bssid) {
  arma_selected[0] = user_in_buff[0];
  arma_selected[1] = user_in_buff[1];

  user_in_buff += 2;
  memcpy(target_bssid, user_in_buff, 12);

  char *b = target_bssid;
  printf(
      "cmd_parser.set_arma_and_target > Target set AP: "
      "%c%c%c%c%c%c%c%c%c%c%c%c\n",
      b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7], b[8], b[9], b[10], b[11]);
  printf("cmd_parser.set_arma_and_target > Armament set: %c%c\n",
         arma_selected[0], arma_selected[1]);
}

void set_arma_selected(char *user_in_buff, char *arma_selected) {
  arma_selected[0] = user_in_buff[0];
  arma_selected[1] = user_in_buff[1];
}

void cmd_ctrl_input_activate(char *user_in_buff, char *arma_selected,
                             char *target_bssid) {
  armament_cmd_event_data cmd_event_data = {.armament_activate = 1};
  memcpy(cmd_event_data.arma_selected, arma_selected, 2);
  memcpy(cmd_event_data.target_bssid, target_bssid, 12);

  size_t event_data_size = sizeof(cmd_event_data);
  ESP_ERROR_CHECK(esp_event_post(ARMAMENT_CMD_EVENT_BASE, CMD_EVENT,
                                 &cmd_event_data, event_data_size,
                                 portMAX_DELAY));

  clear_user_in_buff(user_in_buff);
}

void cmd_ctl_input_deactivate(char *user_in_buff, char *arma_selected,
                              char *target_bssid) {
  armament_cmd_event_data cmd_event_data = {.armament_activate = 0};
  memcpy(cmd_event_data.arma_selected, arma_selected, 2);
  memcpy(cmd_event_data.target_bssid, target_bssid, 12);

  size_t event_data_size = sizeof(cmd_event_data);
  ESP_ERROR_CHECK(esp_event_post(ARMAMENT_CMD_EVENT_BASE, CMD_EVENT,
                                 &cmd_event_data, event_data_size,
                                 portMAX_DELAY));

  clear_user_in_buff(user_in_buff);
  clear_arma_and_bssid_buff(arma_selected, target_bssid);
}

void cmd_instruction_input(char *user_in_buff, char *arma_selected,
                           char *target_bssid) {
  if (memcmp(user_in_buff, PMKID, 2) == 0) {
    printf("cmd_parser.cmd_instruction_input > Using PMKID based attack\n");
  } else if (memcmp(user_in_buff, MIC, 2) == 0) {
    printf("cmd_parser.cmd_instruction_input > Using MIC based attack\n");
  } else if (memcmp(user_in_buff, DEAUTH, 2) == 0) {
    printf("cmd_parser.cmd_instruction_input > Using DEAUTH attack\n");
  }

  if (memcmp(user_in_buff, RECONNAISSANCE, 2) == 0) {
    printf("cmd_parser.cmd_instruction_input > Using RECONNAISSANCE\n");
    set_arma_selected(user_in_buff, arma_selected);
  } else {
    set_arma_and_target(user_in_buff, arma_selected, target_bssid);
  }
  clear_user_in_buff(user_in_buff);
}

void cmd_parser() {
  char user_in_buff[14];
  user_in_buff[0] = '0';
  user_in_buff[1] = '0';

  char arma_selected[2];
  char target_bssid[12];

  while (1) {
    vTaskDelay(CMD_INPUT_DELAY / portTICK_PERIOD_MS);
    scanf("%14s", user_in_buff);

    if (memcmp(user_in_buff, ARMA_STATUS, 2) == 0) {
      output_arma_status(arma_selected, target_bssid);
      clear_user_in_buff(user_in_buff);
    } else if (memcmp(user_in_buff, ARMA_ACTIVATE, 2) == 0) {
      cmd_ctrl_input_activate(user_in_buff, arma_selected, target_bssid);
    } else if (memcmp(user_in_buff, ARMA_DEACTIVATE, 2) == 0) {
      cmd_ctl_input_deactivate(user_in_buff, arma_selected, target_bssid);
    } else if (memcmp(user_in_buff, NULL_ARMA, 2) == 0) {
      continue;
    } else {
      cmd_instruction_input(user_in_buff, arma_selected, target_bssid);
    }
  }
}

void cmd_parser_create_task() {
  arma_cmd_event_register();
  xTaskCreatePinnedToCore(cmd_parser, CMD_PARSER_TASK_NAME,
                          CMD_PARSER_STACK_SIZE, NULL, CMD_PARSER_TASK_PRIORITY,
                          &cmd_parser_task_handle, CMD_PARSER_CORE_TO_USE);
  printf("cmd_parser.cmd_parser_create_task > Cmd parser task created\n");
}