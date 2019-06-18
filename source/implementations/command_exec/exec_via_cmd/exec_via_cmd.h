#pragma once

#include <stdlib.h>
#include <string.h>
#include "../debug_print/debug_print.h"


// Starts cmd.exe and executes the command speficied.
void exec_via_cmd(const char *command) {
    DEBUG_PRINT("exec_via_cmd called.\n");
       
    char *cmd_call = "cmd.exe "; 
    char *system_arg = (char *) malloc(strlen(cmd_call) + strlen(command) + 1);    
    strcpy(system_arg, cmd_call);
    strcat(system_arg, command);

    system(system_arg);
}    
