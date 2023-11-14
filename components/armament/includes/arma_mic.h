/**
 * @file arma_mic.h
 * @author johndeweyzxc (johndewey02003@gmail.com)
 * @brief Declaration of methods for orchestrating the MIC attack
 */

#pragma once

#include "esp_event.h"

// * Configuration for the MIC deauth inject task
#define TMDI_NAME "TASK_MIC_INJECT"
#define TMDI_STACK_SIZE 2048
#define TMDI_PRIORITY 2
#define TMDI_CORE_ID 1

/**
 * @brief Converts the hex character array (12 char) into uinsigned integer
 * array (6 bytes) and copies it in the buffer
 * @param *target_bssid the hex character array (12 char) of mac address of the
 * AP
 */
void arma_mic_set_target(char *target_bssid);

/**
 * @brief Callback function for handling MIC attack notification sent by frame
 * parser
 */
void mic_notif(void *args, esp_event_base_t event_base, int32_t event_id,
               void *event_data);

/**
 * @brief Registers event handler for handling MIC attack notification, the
 * handler uses mic_notif() as the callback function
 */
void arma_mic_notif_event_register();

/**
 * @brief Unregisters event handler for handling MIC attack notification, the
 * handler uses mic_notif() as the callback function
 */
void arma_mic_notif_event_unregister();

/**
 * @brief Deletes the MIC deauth inject task
 */
void arma_mic_delete_task_deauth_inject();

/**
 * @brief This function is invoke when the MIC attack finishes or fails
 */
void arma_mic_finishing_sequence();

/**
 * @brief Injects a deauthentication frame every 3 seconds to the AP
 */
void arma_mic_inject_deauth();

/**
 * @brief Launches the MIC attacking sequence
 * @param *channel channel of the access point, the channel can be either
 * between 1 and 13
 */
void arma_mic_launching_sequence(uint8_t channel);

/**
 * @brief Starts the MIC attack by first scanning nearby access point and if one
 * of those access point matched the bssid with the target bssid then it invokes
 * arma_mic_launching_sequence()
 * @param *target_bssidmac address of the target bssid (6 bytes)
 */
void arma_mic(char *target_bssid);