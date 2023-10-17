/*
 * @file arma_mic.h
 * @author johndeweyzxc (johndewey02003@gmail.com)
 * @brief Declaration of methods for outputting formatted data to serial
 */

#pragma once

#include <esp_wifi_types.h>

/*
 * @brief Outputs information about the AP, this is use when scanning access
 * points.
 * @param *ap_record Information about the access point
 */
void output_ap_info(wifi_ap_record_t *ap_record);

/*
 * @brief Outputs the number of found APs after scanning
 * @param num Total found APs
 */
void output_number_of_aps(uint16_t num);