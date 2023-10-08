#pragma once
#include <stdint.h>

#include "esp_wifi_types.h"

enum filter_type { MANAGEMENT, CTRL, DATA };

#define MAX_SCAN_AP 20
typedef struct {
  uint16_t count;
  wifi_ap_record_t ap_record_list[MAX_SCAN_AP];
} ap_list_from_scan_t;

void wifi_scan_aps();

ap_list_from_scan_t *wifi_get_scanned_aps();

void wifi_connect_to_ap(uint8_t *ssid_name, uint8_t ssid_length,
                        uint8_t channel, uint8_t *bssid);

void wifi_disconnect_from_ap(uint8_t *bssid);

void wifi_set_channel(uint8_t channel);

void wifi_set_filter(uint8_t filter_type);

void wifi_sniffer_start();

void wifi_sniffer_stop();

void wifi_app_init();