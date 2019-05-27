#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "../debug_print/debug_print.h"


// gethostbyname evasion technique.
// Attempts to gather information associated with a specified host.
// The gethostbyname function is expected to fail (by returning NULL). If it succeeds instead, the program exits.
//
// arg1:        Specifies the host name to lookup.
// ---
// Link with -lws2_32 switch when compiling
void gethostbyname_sandbox_evasion(char *arg1) {
    DEBUG_PRINT("Applying gethostbyname sandbox evasion technique.\n");
    DEBUG_PRINT("Attempting to retrieve info for hostname %s...\n", arg1);  

    struct hostent *hp = gethostbyname(arg1);

    if(hp != NULL) {
        exit(0);
    }
}
