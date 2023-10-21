/*
 * @file armament.h
 * @author johndeweyzxc (johndewey02003@gmail.com)
 * @brief Declaration of methods for orchestrating wifi attacks
 */

#pragma once
#include "esp_wifi_types.h"

// Armament codes
#define NULL_ARMA "00"
#define RECONNAISSANCE "01"
#define PMKID "02"
#define MIC "03"
#define DEAUTH "04"

// Control codes
#define ARMA_STATUS "05"
#define ARMA_ACTIVATE "06"
#define ARMA_DEACTIVATE "07"
#define ESP_RESET "08"

/*
 * @brief enum for checking if wifi is initialized or not
 */
enum wifi_init_status_code { WIFI_NOT_INITIALIZED, WIFI_INITIALIZED };

/*
 * @brief enum event id for receiving command input
 */
enum armament_event { CMD_EVENT };

/*
 * @brief Event data that command parser passes and use by armament to determine
 * the selected armament and the target bssid
 */
typedef struct {
  uint8_t armament_activate;
  char arma_selected[2];
  char target_bssid[12];
} armament_cmd_event_data;

/*
 * @brief Callback function that arma_cmd_event_register() registers for
 * handling the activation of armament
 */
void armament_activate_or_deactivate(void *args, esp_event_base_t event_base,
                                     int32_t event_id, void *event_data);

/*
 * @brief Registers event handler for handling armament activation, it uses
 * armament_activate() as the callback function
 */
void arma_cmd_event_register();