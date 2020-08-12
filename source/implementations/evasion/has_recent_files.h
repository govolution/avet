#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "../debug_print/debug_print.h"


// Check if Recent Files Folder exists, proceed if this is the case
void has_recent_files() {
    char * recentFiles = getenv("CSIDL_RECENT");

    if (!recentFiles) {
        DEBUG_PRINT("Unable to get CSIDL_RECENT\n");
    }

    DEBUG_PRINT("Applying has_recent_files evasion technique.\n");
    DEBUG_PRINT("Exiting if recent files directory does not exist.\n");
    struct stat stats;

    stat(recentFiles, &stats);

    if (S_ISDIR(stats.st_mode)) {
        DEBUG_PRINT("Proceed!\n");
    } else {
        exit(0);
    }
}
