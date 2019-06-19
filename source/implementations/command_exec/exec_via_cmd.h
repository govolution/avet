#pragma once

#include <stdlib.h>
#include "../debug_print/debug_print.h"


// Starts cmd.exe and executes the command speficied.
void exec_via_cmd(const char *command, int command_size) {
    DEBUG_PRINT("exec_via_cmd called.\n");
    DEBUG_PRINT("Passing command %s to cmd.exe\n", command);
    system(command);
}    
