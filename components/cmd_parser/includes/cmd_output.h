/*
 * @file cmd_output.h
 * @author johndeweyzxc (johndewey02003@gmail.com)
 * @brief Declaration of methods for outputting a formatted data to the serial
 */

#pragma once

/*
 * @brief Outputs the formatted data to the serial console
 * @param *arma_selected the 2 bytes buffer that contains the content of the
 * selected armament
 * @param *target_bssid the 12 bytes buffer that contains the BSSID or the
 * target mac address
 */
void output_arma_status(char *arma_selected, char *target_bssid);