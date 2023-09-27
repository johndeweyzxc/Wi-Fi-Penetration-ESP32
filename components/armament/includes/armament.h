#pragma once
#include "esp_wifi_types.h"

#define AP_SCANNED "AP_SCANNED"

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

enum wifi_init_status_code { WIFI_NOT_INITIALIZED, WIFI_INITIALIZED };

enum armament_event { CMD_EVENT };

typedef struct {
  char *arma_selected;
  char *target_bssid;
} armament_cmd_event_data;

void output_ap(wifi_ap_record_t *ap_record);

void arma_reconnaissance();

void armament_cmd_event_register();

void armament_cmd_event_unregister();