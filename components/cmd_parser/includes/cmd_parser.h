#pragma once

// * Cmd parser task configuration
#define CMD_INPUT_DELAY 1000
#define CMD_PARSER_TASK_NAME "CMD_PARSER_TASK"
#define CMD_PARSER_STACK_SIZE 2048
#define CMD_PARSER_TASK_PRIORITY 2
#define CMD_PARSER_CORE_TO_USE 1

void clear_user_in_buff(char *user_in_buff);

void clear_arma_and_bssid_buff(char *arma_selected, char *target_bssid);

void set_arma_and_target(char *user_in_buff, char *arma_selected,
                         char *target_bssid);

void set_arma_selected(char *user_in_buff, char *arma_selected);

void output_arma_status(char *arma_selected, char *target_bssid);

void cmd_parser();

void cmd_parser_create_task();