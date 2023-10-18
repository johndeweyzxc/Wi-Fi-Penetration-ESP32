/*
 * @file armament_interface.h
 * @author johndeweyzxc (johndewey02003@gmail.com)
 * @brief Declaration of methods for orchestrating wifi attacks, this header
 * file is use by cmd parser to forward the command of user to the armament.
 */

#pragma once

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

/*
 * @brief enum event id for receiving command input
 */
enum armament_event { CMD_EVENT };

typedef struct {
  uint8_t armament_activate;
  char arma_selected[2];
  char target_bssid[12];
} armament_cmd_event_data;

/*
 * @brief Registers event handler for handling armament activation, it uses
 * armament_activate() as the callback function
 */
void arma_cmd_event_register();