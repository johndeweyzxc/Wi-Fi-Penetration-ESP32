/*
 * @file frame_parser_interface.h
 * @author johndeweyzxc (johndewey02003@gmail.com)
 * @brief Provides the interface for armament to start and stop the processing
 * of incoming frames. It also provides interface for setting the target bssid
 * and setting what type of data to process
 */

#pragma once
#include <stdint.h>

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
 * @brief Used by pmkid_attack_notify_armament() as an event id for notifying
 * the armament about the status of the attack, this is use when armament is
 * PMKID or MIC
 */
enum arma_atk_notify_status_event_id { ATK_STATS_EVENT_ID };

/*
 * @brief Used by pmkid_attack_notify_armament() as part of event data to
 * differentiate between PMKID and MIC based attack
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
} arma_atk_event_data_t;

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
 * @param *target_bssid mac address of the target (6 bytes)
 * @param selected_parse_type data to parse, PARSE_PMKID or PARSE_MIC
 */
void frame_parser_set_target_parameter(uint8_t *target_bssid,
                                       uint8_t selected_parse_type);