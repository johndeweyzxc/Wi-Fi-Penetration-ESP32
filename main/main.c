/**
 * @file main.c
 * @author johndeweyzxc (johndewey02003@gmail.com)
 * @brief Entrypoint of the app, app_main() creates default event loop and also
 * creates task for receiving command input
 */

#include <stdio.h>

#include "cmd_parser_interface.h"
#include "esp_err.h"
#include "esp_event.h"

void app_main(void) {
  ESP_ERROR_CHECK(esp_event_loop_create_default());
  cmd_parser_create_task();
  printf("{ESP_STARTED,}\n");
}
