#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "../debug_print/debug_print.h"


// Check if Public Desktop exists, proceed if this is the case
void has_public_desktop(char *arg1) {
    DEBUG_PRINT("Applying has_public_desktop evasion technique.\n");
    DEBUG_PRINT("Exiting if public desktop does not exists.\n");

    struct stat stats;

    stat("c:\\Users\\Public\\Desktop", &stats);

    if (S_ISDIR(stats.st_mode)) {
        DEBUG_PRINT("Proceed!\n");
    } else {
        exit(0);
    }
}
