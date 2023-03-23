#pragma once

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "../debug_print/debug_print.h"


// Exits if debugger is present
void is_debugger_present(char *arg1) {
   
    if(!IsDebuggerPresent()) {
        DEBUG_PRINT("Proceed!\n");
    } else {
        exit(0);
    }
}