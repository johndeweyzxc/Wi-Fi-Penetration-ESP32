/*
 * @file arma_pmkid.h
 * @author johndeweyzxc (johndewey02003@gmail.com)
 * @brief Declaration of methods for orchestrating the PMKID attack
 */

#pragma once

#include <stdint.h>

#include "esp_event.h"

// Configuration for the PMKID sniff duration task
#define TPS_NAME "TASK_PMKID_SNIFF"
#define TPS_STACK_SIZE 2048
#define TPS_PRIORITY 2
#define TPS_CORE_ID 1

/*
 * @brief Callback function for handling PMKID attack notification sent by frame
 * parser
 */
void pmkid_notif(void *args, esp_event_base_t event_base, int32_t event_id,
                 void *event_data);

/*
 * @brief Registers event handler for handling PMKID attack notification, the
 * handler uses pmkid_notif() as the callback function
 */
void arma_pmkid_notif_event_register();

/*
 * @brief Unregisters event handler for handling PMKID attack notification, the
 * handler uses pmkid_notif() as the callback function
 */
void arma_pmkid_notif_event_unregister();

/*
 * @brief Deletes the PMKID sniff duration task
 */
void arma_delete_task_pmkid_sniff_duration();

/*
 * @brief This function is invoke when the PMKID attack finishes or fails
 */
void arma_pmkid_finishing_sequence();

/*
 * @brief Countdown timer that waits for a few seconds, if it finishes then it
 * means the PMKID attack failed
 */
void arma_pmkid_sniff_duration();

/*
 * @brief Launches the PMKID attacking sequence
 * @param *ssid_name name of access point (1 to 32 characters)
 * @param ssid_len length of the ssid name
 * @param channel channel of the access point, the channel is can be either
 * between 1 and 13
 */
void arma_pmkid_launching_sequence(uint8_t *ssid_name, uint8_t ssid_len,
                                   uint8_t channel);

/*
 * @brief Utility function for calculating the length of the name of access
 * point
 * @param *ssid_name name of access point (1 to 32 characters)
 */
uint8_t calc_len_ssid_name(uint8_t *ssid_name);

/*
 * @brief Starts the PMKID attack by first scanning nearby access point and if
 * one of those access point matched the bssid with the target bssid then it
 * invokes the arma_pmkid_launching_sequence()
 * @param *target_bssid mac address of the target bssid (6 bytes)
 */
void arma_pmkid(char *target_bssid);