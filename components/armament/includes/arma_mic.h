#pragma once

#include "esp_event.h"

// Configuration for the MIC sniff duration task
#define TMS_NAME "TASK_MIC_SNIFF"
#define TMS_STACK_SIZE 2048
#define TMS_PRIORITY 2
#define TMS_CORE_ID 1

/*
 * @brief Callback function for handling MIC attack notification sent by frame
 * parser
 */
static void mic_notif(void *args, esp_event_base_t event_base, int32_t event_id,
                      void *event_data);

/*
 * @brief Registers event handler for handling MIC attack notification, the
 * handler uses mic_notif() as the callback function
 */
void arma_mic_notif_event_register();

/*
 * @brief Unregisters event handler for handling MIC attack notification, the
 * handler uses mic_notif() as the callback function
 */
void arma_mic_notif_event_unregister();

/*
 * @brief Deletes the MIC sniff duration task
 */
void arma_delete_task_mic_sniff_duration();

/*
 * @brief This function is invoke when the MIC attack finishes, it clears the
 * target parameter, unregisters event data frame handler in
 * frame parser. Then it unregisters event handler mic_notif() and finally it
 * stops wifi promiscuous mode
 * @param from_sniff_task determines if the invoker of this function is coming
 * from the arma_mic_sniff_duration(), if it is then there is no need to
 * invoke arma_delete_task_mic_sniff_duration() since the deletion of task
 * arma_mic_sniff_duration() happens inside arma_mic_sniff_duration()
 */
void arma_mic_finishing_sequence(uint8_t from_sniff_task);

/*
 * @brief Countdown timer that waits for a few seconds, if it finishes then it
 * means the MIC attack failed which then invokes
 * arma_mic_finishing_sequence(). This function can skip the countdown if
 * the sniffing of MIC is successful because the mic_notif() will
 * invoke arma_mic_finishing_sequence() which then calls
 * arma_delete_task_mic_sniff_duration()
 */
void arma_mic_sniff_duration();

/*
 * @brief Launches the MIC attack sequence by setting the target parameter and
 * registering event data frame handler in frame parser. Then it registers event
 * handler mic_notif() for handling MIC attack notification. It starts wifi
 * promiscuous mode, sets the wifi filter to data and sets the channel of wifi
 * based on what the access point uses then it connects to the access point and
 * finally it creates the task arma_mic_sniff_duration()
 * @param *channel channel of the access point, the channel can be either
 * between 1 and 13
 */
void arma_mic_launching_sequence(uint8_t channel);

/*
 * @brief Starts the MIC attack by first scanning nearby access point and if one
 * of those access point matched the bssid with the target bssid then it invokes
 * arma_mic_launching_sequence()
 * @param *target_bssid buffer of 6 bytes mac address of the target bssid
 */
void arma_mic(char *target_bssid);