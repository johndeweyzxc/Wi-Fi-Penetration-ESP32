/*
 * @file cmd_parser.c
 * @author johndeweyzxc (johndewey02003@gmail.com)
 * @brief Implements functionality for parsing the input command
 */

#include "cmd_parser.h"

#include <stdio.h>
#include <string.h>

#include "armament_interface.h"
#include "esp_event.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

ESP_EVENT_DECLARE_BASE(ARMAMENT_CMD_EVENT_BASE);
static TaskHandle_t cmd_parser_task_handle = NULL;

void clear_user_in_buff(char *user_in_buff) {
  for (uint8_t i = 0; i < 14; i++) {
    user_in_buff[i] = '0';
  }
  printf("cmd_parser.clear_user_in_buff > *\n");
}

void clear_arma_and_bssid_buff(char *arma_selected, char *target_bssid) {
  arma_selected[0] = '0';
  arma_selected[1] = '0';

  for (uint8_t i = 0; i < 6; i++) {
    target_bssid[i] = '0';
  }
  printf("cmd_parser.clear_arma_and_bssid_buff > *\n");
}

void set_arma_and_target(char *user_in_buff, char *arma_selected,
                         char *target_bssid) {
  arma_selected[0] = user_in_buff[0];
  arma_selected[1] = user_in_buff[1];

  user_in_buff += 2;
  memcpy(target_bssid, user_in_buff, 12);
  printf("cmd_parser.set_arma_and_target > Set target bssid: %s\n",
         target_bssid);
}

void set_arma_selected(char *user_in_buff, char *arma_selected) {
  arma_selected[0] = user_in_buff[0];
  arma_selected[1] = user_in_buff[1];
}

void output_arma_status(char *arma_selected, char *target_bssid) {
  if (arma_selected[0] == '0' && arma_selected[1] == '1') {
    printf("{CURRENT_ARMA,%c%c,}\n", arma_selected[0], arma_selected[1]);
  } else {
    printf("{CURRENT_ARMA,%c%c,%s,}\n", arma_selected[0], arma_selected[1],
           target_bssid);
  }
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
      printf("cmd_parser.cmd_parser > Armament activate!\n");
      arma_cmd_event_register();

      armament_cmd_event_data cmd_event_data = {
          .arma_selected = arma_selected,
          .target_bssid = target_bssid,
      };
      size_t event_data_size = sizeof(cmd_event_data);
      ESP_ERROR_CHECK(esp_event_post(ARMAMENT_CMD_EVENT_BASE, CMD_EVENT,
                                     &cmd_event_data, event_data_size,
                                     portMAX_DELAY));
      clear_user_in_buff(user_in_buff);

    } else if (memcmp(user_in_buff, ARMA_DEACTIVATE, 2) == 0) {
      // TODO: Call function that stops the armament and also clear the
      // selected arma and target bssid buff

      clear_user_in_buff(user_in_buff);
      clear_arma_and_bssid_buff(arma_selected, target_bssid);
      printf("cmd_parser.cmd_parser > Armament deactivated!\n");

    } else if (memcmp(user_in_buff, NULL_ARMA, 2) == 0) {
      continue;
    } else {
      if (memcmp(user_in_buff, RECONNAISSANCE, 2) == 0) {
        set_arma_selected(user_in_buff, arma_selected);
      } else {
        set_arma_and_target(user_in_buff, arma_selected, target_bssid);
      }
      clear_user_in_buff(user_in_buff);
    }
  }
}

void cmd_parser_create_task() {
  xTaskCreatePinnedToCore(cmd_parser, CMD_PARSER_TASK_NAME,
                          CMD_PARSER_STACK_SIZE, NULL, CMD_PARSER_TASK_PRIORITY,
                          &cmd_parser_task_handle, CMD_PARSER_CORE_TO_USE);
  printf("cmd_parser.cmd_parser_create_task > Cmd parser task created\n");
}