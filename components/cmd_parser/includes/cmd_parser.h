/*
 * @file cmd_parser.h
 * @author johndeweyzxc (johndewey02003@gmail.com)
 * @brief Declaration of methods for parsing input command
 */

#pragma once

// Command input task configuration
#define CMD_INPUT_DELAY 1000
#define CMD_PARSER_TASK_NAME "CMD_PARSER_TASK"
#define CMD_PARSER_STACK_SIZE 2048
#define CMD_PARSER_TASK_PRIORITY 2
#define CMD_PARSER_CORE_TO_USE 1

/*
 * @brief Clears the input buffer
 * @param *user_in_buff buffer to store user command input (14 bytes)
 */
void clear_user_in_buff(char *user_in_buff);

/*
 * @brief Clears selected armament and target bssid buffer
 * @param *arma_selected buffer of the selected armament (2 bytes)
 * @param *target_bssid buffer of the target bssid (12 bytes)
 */
void clear_arma_and_bssid_buff(char *arma_selected, char *target_bssid);

/*
 * @brief Puts the target bssid and selected armament into their respective
 * buffer from user input buffer.
 * @param *user_in_buff source from which the data of target bssid and selected
 * armament is first stored
 * @param *arma_selected buffer to store the selected armament (2 bytes)
 * @param *target_bssid buffer to store the target bssid (12 bytes)
 */
void set_arma_and_target(char *user_in_buff, char *arma_selected,
                         char *target_bssid);

/*
 * @brief Puts the selected armament into the buffer from user input buffer.
 * this function is use for reconnaissance
 * @param *user_in_buff source from which the selected armament is first stored
 * (14 bytes)
 * @param *arma_selected buffer to store the selected armament (2 bytes)
 */
void set_arma_selected(char *user_in_buff, char *arma_selected);

/*
 * @brief Prints the selected armament and target bssid
 * @param *arma_selected the selected armament (2 bytes)
 * @param *target_bssid the mac address of the target bssid (12 bytes)
 */
void output_arma_status(char *arma_selected, char *target_bssid);

/*
 * @brief Activates the armament by posting an event and clears the user_in_buff
 * buffer
 * @param *user_in_buff The raw buffer for storing user input (14 bytes)
 * @param *arma_selected The buffer where the selected armament is stored (2
 * bytes)
 * @param *target_bssid The buffer where the mac address of the target BSSID is
 * stored (12 bytes)
 */
void cmd_ctrl_input_activate(char *user_in_buff, char *arma_selected,
                             char *target_bssid);

/*
 * @brief Deactivates the armament by clearing the user_in_buff, arma_selected
 * and the target_bssid buffer
 * @param *user_in_buff The raw buffer for storing user input (14 bytes)
 * @param *arma_selected The buffer where the selected armament is stored (2
 * bytes)
 * @param *target_bssid The buffer where the mac address of the target BSSID is
 * stored (12 bytes)
 */
void cmd_ctl_input_deactivate(char *user_in_buff, char *arma_selected,
                              char *target_bssid);

/*
 * @brief Processes the instruction codes received via serial input by copying
 * the instruction codes from user_in_buff buffer to arma_selected or
 * target_bssid then it clears the user_in_buff buffer
 * @param *user_in_buff Where the raw instruction codes is stored (14 bytes)
 * @param *arma_selected The buffer to store the armament code (2 bytes)
 * @param *target_bssid The buffer to store the mac address of the target BSSID
 * (12 bytes)
 */
void cmd_instruction_input(char *user_in_buff, char *arma_selected,
                           char *target_bssid);

/*
 * @brief Task that continually scans user command input
 */
void cmd_parser();

/*
 * @brief Creates and configures the user command input scanner task. It also
 * registers the input command handler for armament
 */
void cmd_parser_create_task();