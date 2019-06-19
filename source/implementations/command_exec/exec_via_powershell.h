#pragma once

#include <stdlib.h>
#include <string.h>
#include "../debug_print/debug_print.h"


// Starts powershell.exe and executes the command specified.
void exec_via_powershell(const char *command, int command_size) {
    DEBUG_PRINT("exec_via_powershell called.\n");
       
    char *powershell_call = "powershell.exe "; 
    char *system_arg = (char *) malloc(strlen(powershell_call) + command_size + 1);    
    strcpy(system_arg, powershell_call);
    strcat(system_arg, command);

    DEBUG_PRINT("Passing argument %s to cmd.exe\n", system_arg);

    system(system_arg);
} 
