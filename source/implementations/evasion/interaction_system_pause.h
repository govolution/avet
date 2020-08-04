#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "../debug_print/debug_print.h"


// Execute system("pause"), causing the spawned cmd (and our main process) to wait for any keypress.
//
// arg1:        not used
void interaction_system_pause(char *arg1) {
    DEBUG_PRINT("Calling system(pause)...\n");
    system("pause");
    DEBUG_PRINT("Proceeding...");
}
