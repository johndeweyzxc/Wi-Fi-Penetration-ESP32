/*
 * @file arma_utils.c
 * @author johndeweyzxc (johndewey02003@gmail.com)
 * @brief Declaration of methods for utils such as formatting outputs
 */

#pragma once
#include "esp_wifi_types.h"

/*
 * @brief Utility function for appending two string
 */
char *string_append(char *s1, char *s2);

/*
 * @brief Utility function for converting hex string to unsigned 8 bit integer
 */
uint8_t convert_to_uint8_t(char s1, char s2);

/*
 * @brief Outputs access point and their ssid, this is use when scanning access
 * points.
 * @param *ap_record Information about the access point
 */
void output_ap_info(wifi_ap_record_t *ap_record);