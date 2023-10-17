#include "cmd_output.h"

#include <stdio.h>

/*
 * @file cmd_output.c
 * @author johndeweyzxc (johndewey02003@gmail.com)
 * @brief Implementation of methods for outputting a formatted data to the
 * serial
 */

void output_arma_status(char *arma_selected, char *target_bssid) {
  if (arma_selected[0] == '0' && arma_selected[1] == '1') {
    printf("{CURRENT_ARMA,%c%c,}\n", arma_selected[0], arma_selected[1]);
  } else {
    char *b = target_bssid;
    printf("{CURRENT_ARMA,%c%c,%c%c%c%c%c%c%c%c%c%c%c%c,}\n", arma_selected[0],
           arma_selected[1], b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7],
           b[8], b[9], b[10], b[11]);
  }
}