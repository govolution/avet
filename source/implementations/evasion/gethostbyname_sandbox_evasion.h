#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "../debug_print/debug_print.h"

// evading with gethostbyname technique
// arg1 serves no purpose and is ignored
// you can set a define HOSTVALUE to specify which host is used in the gethostbyname call.
// else the localhost is used.
// ---
// Link with -lws2_32 switch when compiling
void gethostbyname_sandbox_evasion(char *arg1) {
    DEBUG_PRINT("Using gethostbyname sandbox evasion technique...\n");
   
    #ifdef HOSTVALUE
        struct hostent *hp = gethostbyname(HOSTVALUE);
    #else
        struct hostent *hp = gethostbyname("localhost");    
    #endif
    if (hp != NULL) { 		
	    exit(0);
    }
}
