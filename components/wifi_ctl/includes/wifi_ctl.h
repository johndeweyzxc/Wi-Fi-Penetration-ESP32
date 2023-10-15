/*
 * @file wifi_ctl.h
 * @author johndeweyzxc (johndewey02003@gmail.com)
 * @brief Declaration of methods for controlling the wifi
 */

#pragma once
#include <stdint.h>

#include "esp_wifi_types.h"

/*
 * @brief Type of filters available to use as arguments for wifi_set_filter()
 */
enum filter_type { MANAGEMENT, CTRL, DATA };

/*
 * @brief Event id that received_frame() uses to tell frame parser the type of
 * frame
 */
enum frame_event_id { DATA_FRAME, MGMT_FRAME, CTRL_FRAME };

/*
 * @brief Total maximum number of access points to scan
 */
#define MAX_SCAN_AP 20

/*
 * @brief wifi_get_scan_aps() returns this struct
 */
typedef struct {
  uint16_t count;
  wifi_ap_record_t ap_record_list[MAX_SCAN_AP];
} ap_list_from_scan_t;

/*
 * @brief Injects raw arbitrary 802.11 frame
 * @param *frame_buffer Buffer that contains the frame to be injected
 * @param size Size of the frame buffer
 */
void wifi_inject_frame(uint8_t *frame_buffer, int size);

/*
 * @brief Scans a maximum of 20 access points
 */
void wifi_scan_aps();

/*
 * @brief Returns the scanned access points and their information
 * @return ap_list_from_scan_t* which contains the number of scanned access
 * points and the list of access points and their information
 */
ap_list_from_scan_t *wifi_get_scanned_aps();

/*
 * @brief Connects to the access point, this function is use when armament is
 * PMKID
 * @param ssid_name name of the wifi
 * @param ssid_length length of ssid name, maximum allowed length is
 * 1 to 32
 * @param channel the channel to be use which can be anything from 1 to 13
 * @param bssid mac address of the wifi which is 6 bytes
 */
void wifi_connect_to_ap(uint8_t *ssid_name, uint8_t ssid_length,
                        uint8_t channel, uint8_t *bssid);

/*
 * @brief Disconnects from the access point, this function is use when armament
 * is PMKID
 * @param bssid mac address of the wifi which is 6 bytes
 */
void wifi_disconnect_from_ap(uint8_t *bssid);

/*
 * @brief Sets the channel of wifi
 * @param channel the channel to be use which can be anything from 1 to 13
 */
void wifi_set_channel(uint8_t channel);

/*
 * @brief Sets the filter of wifi to Management, Data or Control frames
 * @param filter_type filter type can be (0 - MGMT, 1 - CTRL, and 2 - DATA)
 */
void wifi_set_filter(uint8_t filter_type);

/*
 * @brief Callback function when a frame is received in promiscuous mode
 * @param *buf pointer to the frame
 * @param type type of frame (Management, Control, Data or Misc)
 */
void received_frame(void *buf, wifi_promiscuous_pkt_type_t type);

/*
 * @brief Starts promiscuous mode or monitor mode
 */
void wifi_sniffer_start();

/*
 * @brief Stops promiscuous mode or monitor mode
 */
void wifi_sniffer_stop();

/*
 * @brief Configures and initializes the wifi
 */
void wifi_app_init();