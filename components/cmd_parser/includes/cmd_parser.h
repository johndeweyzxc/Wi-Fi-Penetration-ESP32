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
 * @param *user_in_buff buffer of the user command input
 */
void clear_user_in_buff(char *user_in_buff);

/*
 * @brief Clears selected armament and target bssid buffer
 * @param *arma_selected buffer of the selected armament (2 bytes)
 * @param *target_bssid buffer of the target bssid (12 bytes)
 */
void clear_arma_and_bssid_buff(char *arma_selected, char *target_bssid);

/*
 * @brief Sets the target bssid and selected armament
 * @param *user_in_buff source from which the data of target bssid and selected
 * armament is first stored
 * @param *arma_selected buffer to store the selected armament (2 bytes)
 * @param *target_bssid buffer to store the target bssid (12 bytes)
 */
void set_arma_and_target(char *user_in_buff, char *arma_selected,
                         char *target_bssid);

/*
 * @brief Sets the selected armament, this function is use for reconnaissance
 * @param *user_in_buff source from which the selected armament is first stored
 * @param *arma_selected buffer to store the selected armament (2 bytes)
 */
void set_arma_selected(char *user_in_buff, char *arma_selected);

/*
 * @brief Prints the selected armament and target bssid
 * @param *arma_selected the selected armament
 * @param *target_bssid the mac address of the target bssid
 */
void output_arma_status(char *arma_selected, char *target_bssid);

/*
 * @brief Task that continually scans user command input
 */
void cmd_parser();

/*
 * @brief Creates and configures the user command input scanner task
 */
void cmd_parser_create_task();