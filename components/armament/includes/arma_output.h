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

/*
 * @brief Outputs the MIC attack operation as started
 * @param *target_bssid mac address (6 bytes) of the target AP
 */
void output_mic_operation_started(uint8_t *target_bssid);

/*
 * @brief Indicates MIC launching sequence
 */
void output_mic_launching_sequence();

/*
 * @brief Outputs the elapsed time since the MIC deauth task was created
 * @param current_time the current time that elapsed
 */
void output_mic_deauth_status(int current_time);

/*
 * @brief Indicates the MIC finishing sequence
 */
void output_mic_finishing_sequence();

/*
 * @brief Outputs the PMKID attack operation as success
 * @param *target_bssid mac address (6 bytes) of the target AP
 */
void output_success_mic_attack(uint8_t *target_bssid);

/*
 * @brief Outputs the MIC attack operation as failure
 * @param *target_bssid mac address (6 bytes) of the target AP
 */
void output_failed_mic_attack(uint8_t *target_bssid);

/*
 * @brief Outputs the PMKID attack operation as started
 * @param *target_bssid mac address (6 bytes) of the target AP
 */
void output_pmkid_operation_started(uint8_t *target_bssid);

/*
 * @brief Indicates that the PMKID attack failed because the target AP is not
 * found
 * @param *target_bssid mac address (6 bytes) of the target AP
 */
void output_failed_pmkid_not_found_ap(uint8_t *target_bssid);

/*
 * @brief Outputs the MIC attack operation as failure
 * @param *target_bssid mac address (6 bytes) of the target AP
 */
void output_success_pmkid_attack(uint8_t *target_bssid);

/*
 * @brief Indicates the PMKID launching sequence
 */
void output_pmkid_launching_sequence();

/*
 * @brief Outputs the time elapsed since the PMKID task started
 * @param current_time the current time that elapsed
 */
void output_pmkid_sniff_status(int current_time);

/*
 * @brief Indicates the pmkid finishing sequence
 */
void output_pmkid_finishing_sequence();

/*
 * @brief Outputs the PMKID attack operation as failure
 * @param *target_bssid mac address (6 bytes) of the target AP
 */
void output_failed_pmkid_attack(uint8_t *target_bssid);

/*
 * @brief Indicates the DEAUTH launching sequence
 */
void output_deauth_launching_sequence();

/*
 * @brief Indicates that the deauth attack has started
 * @param *target_bssid mac address (6 bytes) of the target AP
 */
void output_started_deauth(uint8_t *target_bssid);

/*
 * @brief Outputs the time elapsed since the DEAUTH task started
 * @param current_time the current time that elapsed
 */
void output_deauth_status(int current_time);

/*
 * @brief Indicates that the deauth attack failed because the target AP is not
 * found
 * @param *target_bssid mac address (6 bytes) of the target AP
 */
void output_failed_deauth_not_found_ap(uint8_t *target_bssid);

/*
 * @brief Indicates that the deauth attack has stopped
 * @param *target_bssid mac address (6 bytes) of the target AP
 */
void output_stopped_deauth(uint8_t *target_bssid);

/*
 * @brief Indicates that the armament will be activated
 */
void output_armament_activation();

/*
 * @brief Indicates that the armament will be deactivated
 */
void output_armament_deactivation();