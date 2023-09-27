#pragma once

// * Armament codes
#define NULL_ARMA "00"
#define RECONNAISSANCE "01"
#define PMKID "02"
#define MIC "03"
#define DEAUTH "04"

// * Control codes
#define ARMA_STATUS "05"
#define ARMA_ACTIVATE "06"
#define ARMA_DEACTIVATE "07"

enum armament_event { CMD_EVENT };

typedef struct {
  char *arma_selected;
  char *target_bssid;
} armament_cmd_event_data;

void armament_cmd_event_register();

void armament_cmd_event_unregister();