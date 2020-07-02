#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "../debug_print/debug_print.h"


// Sleep for a specified time
//
// arg1:        time in MilliSeconds
void evasion_by_sleep(char *arg1) {
    DEBUG_PRINT("Applying evasion_by_sleep technique.\n");
    DEBUG_PRINT("Sleeping for %s...\n", arg1);

    int time = atoi(arg1);
    Sleep(time);
    DEBUG_PRINT("Proceed!\n");
}
