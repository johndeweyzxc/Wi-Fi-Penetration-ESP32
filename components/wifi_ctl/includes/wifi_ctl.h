#pragma once
#include <stdint.h>

#include "esp_event.h"
#include "esp_wifi_types.h"

ESP_EVENT_DECLARE_BASE(FRAME_RECEIVED_EVENT_BASE);

enum filter_type { MANAGEMENT, CTRL, DATA };

enum frame_event_id { DATA_FRAME, MGMT_FRAME, CTRL_FRAME };

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

static void received_frame(void *buf, wifi_promiscuous_pkt_type_t type);

void wifi_sniffer_start();

void wifi_sniffer_stop();

void wifi_app_init();