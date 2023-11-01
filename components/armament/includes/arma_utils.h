/*
 * @file arma_utils.h
 * @author johndeweyzxc (johndewey02003@gmail.com)
 * @brief Declaration of methods for converting a hexadecimal string to unsigned
 * 8 bit integer
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
 * @brief Utility function for printing an array of character into hexadecimal
 * string
 */
void print_string_into_hex(uint8_t *ssid);