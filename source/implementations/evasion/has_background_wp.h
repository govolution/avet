#pragma once

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "../debug_print/debug_print.h"



void has_background_wp() {
    DEBUG_PRINT("Applying has_background_wp evasion technique.\n");
    DEBUG_PRINT("Exiting if background wallpaper is not set\n");

   if( access("%APPDATA%\\Microsoft\\Windows\\Themes\\TranscodedWallpaper", F_OK ) != -1 ) {
       DEBUG_PRINT("Proceed!\n");
    } else {
        exit(0);
    }
}