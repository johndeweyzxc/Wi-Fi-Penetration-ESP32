/*
 * @file arma_utils.c
 * @author johndeweyzxc (johndewey02003@gmail.com)
 * @brief Declaration of methods for utils such as formatting outputs
 */

#pragma once
#include "esp_wifi_types.h"

/*
 * @brief Outputs access point and their ssid, this is use when scanning access
 * points.
 * @param *ap_record Information about the access point
 */
void output_ap_info(wifi_ap_record_t *ap_record);