/**
 * @file frame_constants.h
 * @author johndeweyzxc (johndewey02003@gmail.com)
 * @brief Declaration of a frame structure that is found on all types of frame
 */

#pragma once

/**
 * @brief Frame control (2 bytes)
 */
typedef struct __attribute__((__packed__)) {
  uint8_t protocol_version : 2;
  uint8_t type : 2;
  uint8_t subtype : 4;
  uint8_t ds_status : 2;
  uint8_t more_fragments : 1;
  uint8_t retry : 1;
  uint8_t power_management : 1;
  uint8_t more_data : 1;
  uint8_t protected_frame : 1;
  uint8_t htc_order : 1;
} frame_control_t;

/**
 * @brief Mac header (18 bytes)
 */
typedef struct {
  uint8_t receiver_addr[6];
  uint8_t transmitter_addr[6];
  uint8_t bssid[6];
} mac_header_t;