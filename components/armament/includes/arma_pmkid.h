#pragma once

#include <stdint.h>

#include "esp_event.h"

// * Task Pmkid Sniff configuration
#define TPS_NAME "TASK_PMKID_SNIFF"
#define TPS_STACK_SIZE 2048
#define TPS_PRIORITY 2
#define TPS_CORE_ID 1

static void attack_notification(void *args, esp_event_base_t event_base,
                                int32_t event_id, void *event_data);

void arma_pmkid_notif_event_register();

void arma_pmkid_notif_event_unregister();

void delete_arma_task_pmkid_sniff_duration();

void arma_pmkid_attack_cleanup_sequence(uint8_t from_sniff_task);

void arma_pmkid_sniff_duration();

void arma_launch_pmkid_attack_sequence(uint8_t *ssid_name, uint8_t ssid_len,
                                       uint8_t channel);

uint8_t calc_len_ssid_name(uint8_t *ssid_name);

char *string_append(char *s1, char *s2);

uint8_t convert_to_uint8_t(char s1, char s2);

void arma_pmkid(char *target_bssid);