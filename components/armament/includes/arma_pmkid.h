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
 * @brief This function is invoke when the PMKID attack finishes, it clears the
 * target parameter, unregisters event data frame handler in
 * frame parser. Then it unregisters event handler pmkid_notif(), stops wifi
 * promiscuous mode and finally it disconnects from the access point
 * @param from_sniff_task determines if the invoker of this function is coming
 * from the arma_pmkid_sniff_duration(), if it is then there is no need to
 * invoke arma_delete_task_pmkid_sniff_duration() since the deletion of task
 * arma_pmkid_sniff_duration() happens inside arma_pmkid_sniff_duration()
 */
void arma_pmkid_finishing_sequence(uint8_t from_sniff_task);

/*
 * @brief Countdown timer that waits for a few seconds, if it finishes then it
 * means the PMKID attack failed which then invokes
 * arma_pmkid_finishing_sequence(). This function can skip the countdown if
 * the sniffing of PMKID is successful because the pmkid_notif() will
 * invoke arma_pmkid_finishing_sequence() which then calls
 * arma_delete_task_pmkid_sniff_duration()
 */
void arma_pmkid_sniff_duration();

/*
 * @brief Launches the PMKID attack sequence by setting the target parameter and
 * registering event data frame handler in frame parser. Then it registers event
 * handler pmkid_notif() for handling PMKID attack notification. It starts wifi
 * promiscuous mode, sets the wifi filter to data and sets the channel of wifi
 * based on what the access point uses then it connects to the access point and
 * finally it creates the task arma_delete_task_pmkid_sniff_duration()
 * @param *ssid_name name of access point
 * @param ssid_len length of the ssid name
 * @param channel channel of the access point, the channel is can be either
 * between 1 and 13
 */
void arma_pmkid_launching_sequence(uint8_t *ssid_name, uint8_t ssid_len,
                                   uint8_t channel);

/*
 * @brief Utility function for calculating the length of the name of access
 * point
 */
uint8_t calc_len_ssid_name(uint8_t *ssid_name);

/*
 * @brief Starts the PMKID attack by first scanning nearby access point and if
 * one of those access point matched the bssid with the target bssid then it
 * invokes the arma_pmkid_launching_sequence()
 * @param *target_bssid buffer of 6 bytes mac address of the target bssid
 */
void arma_pmkid(char *target_bssid);