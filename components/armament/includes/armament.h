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

// * Task Pmkid Sniff configuration
#define TPS_NAME "TASK_PMKID_SNIFF"
#define TPS_STACK_SIZE 2048
#define TPS_PRIORITY 2
#define TPS_CORE_ID 1

enum wifi_init_status_code { WIFI_NOT_INITIALIZED, WIFI_INITIALIZED };

enum armament_event { CMD_EVENT };

typedef struct {
  char *arma_selected;
  char *target_bssid;
} armament_cmd_event_data;

void output_ap(wifi_ap_record_t *ap_record);

void arma_reconnaissance();

char *string_append(char *s1, char *s2);

uint8_t convert_to_uint8_t(char s1, char s2);

uint8_t calc_length_of_ssid_name(uint8_t *ssid_name);

void arma_pmkid_sniff_duration();

void delete_arma_task_pmkid_sniff_duration();

void arma_pmkid(char *target_bssid);

void armament_cmd_event_register();

void armament_cmd_event_unregister();

void handle_post_pmkid_attack();

void handle_post_mic_attack();

void armament_attack_notification_event_register();

void armament_attack_notification_event_unregister();