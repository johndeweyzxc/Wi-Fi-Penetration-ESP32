/**
 * @file frame_bypasser.h
 * @author johndeweyzxc (johndewey02003@gmail.com)
 * @brief Implements the function for injecting arbitrary 802.11 frame
 */

#include "frame_bypasser.h"

#include "esp_err.h"
#include "esp_wifi.h"

int ieee80211_raw_frame_sanity_check(int32_t arg, int32_t arg2, int32_t arg3) {
  return 0;
}

void frame_bypasser_inject_frame(uint8_t *frame_buffer, int size) {
  ESP_ERROR_CHECK(esp_wifi_80211_tx(WIFI_IF_STA, frame_buffer, size, false));
}