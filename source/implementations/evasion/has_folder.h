#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "../debug_print/debug_print.h"

// This fuction checks if a folder exists.
//
// arg1:  path to folder
// must be in unix style, because "\" makes problems
void has_folder(char *arg1) {
    DEBUG_PRINT("Applying has_folder evasion technique.\n");
    DEBUG_PRINT("Exiting if %s does not exists.\n", arg1);

    struct stat stats;

    stat(arg1, &stats);

    if (S_ISDIR(stats.st_mode)) {
        DEBUG_PRINT("Proceed!\n");
    } else {
        exit(0);
    }
}
