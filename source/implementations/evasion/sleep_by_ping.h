#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../debug_print/debug_print.h"


// Halt execution of the program for arg1 seconds by invoking a timed ping command against localhost.
// Will ping once each second, for arg1 times.
//
// arg1:        ping duration in seconds
void sleep_by_ping(char *arg1) {
    DEBUG_PRINT("Applying sleep_by_ping technique.\n");
    DEBUG_PRINT("Pinging localhost for %s seconds.\n", arg1);

    char *ping_command = "ping 127.0.0.1 -n ";
    char *complete_command = (char *) malloc(strlen(ping_command) + strlen(arg1) + 1);
    strcpy(complete_command, ping_command);
    strcat(complete_command, arg1);
    DEBUG_PRINT("Invoking command %s \n", complete_command);
    system(complete_command);
    free(complete_command);
    DEBUG_PRINT("Ping finished, proceeding...");
}
