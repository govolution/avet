#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "../debug_print/debug_print.h"


// Check if Network Drive exists, proceed if this is the case
void has_network_drive(char *arg1) {
    char * network = getenv("CSIDL_NETWORK");

    if (!network) {
        DEBUG_PRINT("Unable to get CSIDL_NETWORK\n");
    }

    DEBUG_PRINT("Applying has_network_drive evasion technique.\n");
    DEBUG_PRINT("Exiting if recent files directory does not exist.\n");
    struct stat stats;

    stat(network, &stats);

    if (S_ISDIR(stats.st_mode)) {
        DEBUG_PRINT("Proceed!\n");
    } else {
        exit(0);
    }
}
