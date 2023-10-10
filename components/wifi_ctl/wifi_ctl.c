/*
 * @file wifi_ctl.c
 * @author johndeweyzxc (johndewey02003@gmail.com)
 * @brief Implements the functionality for controlling the behaviour of wifi
 */

#include "wifi_ctl.h"

#include <stdio.h>
#include <string.h>

#include "esp_err.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "esp_wifi_types.h"

ESP_EVENT_DECLARE_BASE(FRAME_RECEIVED_EVENT_BASE);
static ap_list_from_scan_t ap_list;

void wifi_scan_aps() {
  ap_list.count = MAX_SCAN_AP;

  wifi_scan_config_t scan_config = {.ssid = NULL,
                                    .bssid = NULL,
                                    .channel = 0,
                                    .scan_type = WIFI_SCAN_TYPE_ACTIVE};

  ESP_ERROR_CHECK(esp_wifi_scan_start(&scan_config, true));
  ESP_ERROR_CHECK(
      esp_wifi_scan_get_ap_records(&ap_list.count, ap_list.ap_record_list));

  printf("wifi_ctl.wifi_scan_aps > Found %u APs\n", ap_list.count);
}

ap_list_from_scan_t *wifi_get_scanned_aps() { return &ap_list; }

void wifi_connect_to_ap(uint8_t *ssid_name, uint8_t ssid_length,
                        uint8_t channel, uint8_t *bssid) {
  if (ssid_length <= 0 || ssid_length > 33) {
    printf("wifi_ctl.wifi_connect_to_ap > Cannot connect to wifi\n");
    printf("wifi_ctl.wifi_connect_to_ap > SSID length is %u\n", ssid_length);
    return;
  }

  printf("wifi_ctl.wifi_connect_to_ap > %02X%02X%02X%02X%02X%02X\n", bssid[0],
         bssid[1], bssid[2], bssid[3], bssid[4], bssid[5]);
  printf("wifi_ctl.wifi_connect_to_ap > SSID is: %s\n", ssid_name);
  printf("wifi_ctl.wifi_connect_to_ap > Channel is %u\n", channel);

  wifi_config_t wifi_config = {
      .sta = {.channel = channel,
              .scan_method = WIFI_FAST_SCAN,
              .pmf_cfg.capable = false,
              .pmf_cfg.required = false},
  };
  memcpy(wifi_config.sta.ssid, ssid_name, ssid_length);
  uint8_t dummy_password[] = "dummy";
  memcpy(wifi_config.sta.password, dummy_password, 6);

  ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
  ESP_ERROR_CHECK(esp_wifi_connect());
}

void wifi_disconnect_from_ap(uint8_t *bssid) {
  ESP_ERROR_CHECK(esp_wifi_disconnect());
  printf("wifi_ctl.wifi_disconnect_from_ap > %02X:%02X:%02X:%02X:%02X:%02X\n",
         bssid[0], bssid[1], bssid[2], bssid[3], bssid[4], bssid[5]);
}

void wifi_set_channel(uint8_t channel) {
  // TODO: Check if channel is between 1 and 14
  esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
  printf("wifi_ctl.wifi_set_channel > Wifi channel set to %u\n", channel);
}

void wifi_set_filter(uint8_t filter_type) {
  wifi_promiscuous_filter_t filter = {.filter_mask = 0};

  switch (filter_type) {
    case MANAGEMENT:
      filter.filter_mask |= WIFI_PROMIS_FILTER_MASK_MGMT;
      printf("wifi_ctl.wifi_set_filter > Filter set to mgt frames\n");
      break;
    case CTRL:
      filter.filter_mask |= WIFI_PROMIS_FILTER_MASK_CTRL;
      printf("wifi_ctl.wifi_set_filter > Filter set to ctrl frames\n");
      break;
    case DATA:
      filter.filter_mask |= WIFI_PROMIS_FILTER_MASK_DATA;
      printf("wifi_ctl.wifi_set_filter > Frame filter set to data frames\n");
      break;
  }

  esp_wifi_set_promiscuous_filter(&filter);
}

static void received_frame(void *buf, wifi_promiscuous_pkt_type_t type) {
  wifi_promiscuous_pkt_t *frame = (wifi_promiscuous_pkt_t *)buf;

  uint8_t event_id;
  switch (type) {
    case WIFI_PKT_DATA:
      event_id = DATA_FRAME;
      break;
    case WIFI_PKT_MGMT:
      event_id = MGMT_FRAME;
      break;
    case WIFI_PKT_CTRL:
      event_id = CTRL_FRAME;
      break;
    default:
      printf("wifi_ctl.received_frame > Unknown frame\n");
      return;
      // TODO: Add case statement for MISC frame type
  }

  size_t data_size = frame->rx_ctrl.sig_len + sizeof(wifi_promiscuous_pkt_t);
  ESP_ERROR_CHECK(esp_event_post(FRAME_RECEIVED_EVENT_BASE, event_id, frame,
                                 data_size, portMAX_DELAY));
}

void wifi_sniffer_start() {
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_promiscuous_rx_cb(&received_frame);
  printf("wifi_ctl.wifi_sniffer_start > Promiscuous mode started\n");
}

void wifi_sniffer_stop() {
  esp_wifi_set_promiscuous(false);
  printf("wifi_ctl.wifi_sniffer_start > Promiscuous mode stopped\n");
}

static void wifi_event_handler(void *event_handler_arg,
                               esp_event_base_t event_base, int32_t event_id,
                               void *event_data) {}

void wifi_app_init() {
  ESP_ERROR_CHECK(esp_netif_init());
  esp_netif_create_default_wifi_sta();

  wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&wifi_init_config));
  ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

  ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID,
                                             &wifi_event_handler, NULL));

  ESP_ERROR_CHECK(esp_wifi_start());
  printf("wifi_ctl.wifi_app_init > Initialized wifi\n");
}