/*
 * @file frame_parser.h
 * @author johndeweyzxc (johndewey02003@gmail.com)
 * @brief Declaration of methods for parsing frame data received from monitor
 * mode of wifi in wifi_ctl
 */

#pragma once
#include <stdint.h>

#include "eapol_frame.h"
#include "esp_event.h"
#include "frame_constants.h"

/*
 * @brief Argument for frame_parser_set_target_parameter(), this is use to
 * determine if armament is PMKID or MIC
 */
enum parse_type_list { NULL_PARSE_TYPE, PARSE_PMKID, PARSE_MIC };

/*
 * @brief Used by frame_parser_register_data_frame_handler() as an event id for
 * determining frame type. Though only DATA_FRAME is the only one being used
 */
enum frame_event_id { DATA_FRAME, MGMT_FRAME, CTRL_FRAME };

/*
 * @brief Used by pmkid_notify_armament() as an event id for notifying
 * the armament about the status of the attack, this is use when armament is
 * PMKID or MIC
 */
enum arma_atk_notify_status_event_id { ATK_STATS_EVENT_ID };

/*
 * @brief Used by pmkid_notify_armament() and mic_notify_armament() as part of
 * event data to differentiate between PMKID and MIC based attack
 */
enum arma_atk_notify_context { PMKID_BASED, MIC_BASED };

/*
 * @brief Event data to notify about the status of the attack, it contains
 * atk_context, which can be either PMKID_BASED or MIC_BASED
 * message_number, first message or second message
 * bssid, mac address of access point
 * station_mac, mac address of station
 */
typedef struct {
  uint8_t atk_context;
  uint8_t message_number;
  uint8_t bssid[6];
  uint8_t station_mac[6];
} arma_atk_event_data_t;

/*
 * @brief Notifies armament about the status of the PMKID attack by posting an
 * event to the event loop
 */
void pmkid_notify_armament();

/*
 * @brief Parses the PMKID and check if valid. If valid, it outputs the PMKID
 * and then it notifies the armament by invoking pmkid_notify_armament()
 * @param *wpa_data wpa data in the eapol frame
 * @param *eapol_frame eapol frame which contains wpa data
 * @param *key_info information about the key to determine if it is a message 1
 * or 2 from 4 way handshake
 */
void parse_pmkid(eapol_auth_data_t *wpa_data, eapol_frame_t *eapol_frame,
                 key_information_t *key_info);

/*
 * @brief Notifies armament about the status of the MIC attack by posting an
 * event to the event loop
 */
void mic_notify_armament(uint8_t message_number, mac_header_t *mac_header);

/*
 * @brief Parses the MIC and check if valid. If valid, it outputs the
 * anonce, bssid and station mac from first eapol message. Then it outputs
 * the authentication data from second eapol message. The armament gets
 * notified by each messages
 * @param *wpa_data wpa data in the eapol frame
 * @param *eapol_frame eapol frame which contains wpa data
 * @param *key_info information about the key to determine if it is a
 * message 1 or 2 from 4 way handshake
 */
void parse_mic(eapol_auth_data_t *wpa_data, eapol_frame_t *eapol_frame,
               key_information_t *key_info);

/*
 * @brief Do a bitwise operation on key information to determine the values of
 * the attributes
 * @param *key_info_16 16 bits of key information and its attributes
 * @return key information which is of type key_information_t
 */
key_information_t perform_bitwise_on_key_info(uint16_t *key_info_16);

/*
 * @brief Checks if armament is PMKID or MIC. Which then calls parse_pmkid() if
 * PMKID or parse_mic() if MIC. This also performs bitwise operation on key
 * information
 * @param *wpa_data pointer to the wpa data in the eapol frame
 * @param *eapol_frame pointer to the eapol frame
 */
void parse_80211_authentication(eapol_auth_data_t *wpa_data,
                                eapol_frame_t *eapol_frame);

/*
 * @brief Callback function for handling frame of type data when promiscuous
 * mode or monitor mode is turned on
 */
static void data_frame_parser(void *args, esp_event_base_t event_base,
                              int32_t event_id, void *event_data);

/*
 * @brief Unregisters event handler data_frame_parser() which handles incoming
 * frame of type data when promiscuous or monitor mode is turned on
 */
void frame_parser_unregister_data_frame_handler();

/*
 * @brief Registers event handler data_frame_parser() which handles incoming
 * frame of type data when promiscuous or monitor mode is turned on
 */
void frame_parser_register_data_frame_handler();

/*
 * @brief Clears the value of target mac address and parse type
 */
void frame_parser_clear_target_parameter();

/*
 * @brief Sets the target mac address and the parse type
 * @param *target_bssid pointer to the 6 bytes array of mac address
 * @param selected_parse_type data to parse, PARSE_PMKID or PARSE_MIC
 */
void frame_parser_set_target_parameter(uint8_t *target_bssid,
                                       uint8_t selected_parse_type);