/*
 * @file wifi_ctl.h
 * @author johndeweyzxc (johndewey02003@gmail.com)
 * @brief Declaration of methods for injecting deauthentication frame
 */

#pragma once

#include <stdint.h>

// Configuration for the DEAUTH interval task
#define TDI_NAME "TASK_DEAUTH_INTERVAL"
#define TDI_STACK_SIZE 2048
#define TDI_PRIORITY 2
#define TDI_CORE_ID 1

/*
 * @brief Loads deauthentication payload into the buffer that will be injected.
 * The payload contains the following: Subtype (1 bytes), Flags (1 bytes),
 * Duration (2 bytes),
 * Receiver address (6 bytes),
 * Source address (6 bytes),
 * BSSID (6 bytes),
 * Sequence number (2 bytes),
 * Reason code (2 bytes)
 * @param *payload_buffer The buffer to store the payload
 */
void load_deauth_payload(uint8_t *payload_buffer);

/*
 * @brief Task that continuously injects the deauthentication payload
 */
void arma_run_deauth();

/*
 * @brief Kills the task arma_run_deauth()
 */
void arma_deauth_finish();

/*
 * @brief Creates task arma_run_deauth() and sets the wifi channel based on the
 * channel of the access point
 * @param channel The channel of access point
 */
void arma_deauth_launching_sequence(uint8_t channel);

/*
 * @brief Sets the target bssid by copying it into the buffer
 * @param target_bssid the buffer of the target bssid
 */
void arma_mic_set_target_bssid(uint8_t *target_bssid);

/*
 * @brief Starts the deauth attack
 */
void arma_deauth(char *target_bssid);