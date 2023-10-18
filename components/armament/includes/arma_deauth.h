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
 * @brief Converts the hex character array (12 char) into uinsigned integer
 * array (6 bytes) and copies it in the buffer
 * @param *target_bssid the hex character array (12 char) of mac address of the
 * AP
 */
void arma_deauth_set_target(char *target_bssid);

/*
 * @brief Loads deauthentication payload into the buffer that will be injected
 */
void load_deauth_payload();

/*
 * @brief Task that continuously injects the deauthentication payload
 */
void arma_run_deauth();

/*
 * @brief Injects a deauthentication frame to the AP
 */
void arma_deauth_inject();

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
 * @brief Starts the deauth attack
 * @param *target_bssid the buffer of the target bssid which contains
 * hexadecimal string (12 bytes)
 */
void arma_deauth(char *target_bssid);