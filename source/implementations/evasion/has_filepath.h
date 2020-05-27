#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "../debug_print/debug_print.h"


void has_background_wp(char *arg1) {
    DEBUG_PRINT("Applying has_recycle_bin evasion technique.\n");
    DEBUG_PRINT("Exiting if recycle bin does not exists.\n");

    struct stat stats;

    stat(arg1, &stats);

    if (S_ISDIR(stats.st_mode)) {
        DEBUG_PRINT("Proceed!\n");
    } else {
        exit(0);
    }
}