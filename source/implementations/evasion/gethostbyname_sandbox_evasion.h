#include <stdio.h>
#include <stdlib.h>

// evading with gethostbyname technique
// arg1 specifies host
// Link with -lws2_32 switch when compiling
void gethostbyname_sandbox_evasion(char *arg1) {
    #ifdef PRINT_DEBUG
	    printf("use  gethostbyname sandbox evasion\n");
    #endif
    struct hostent *hp = gethostbyname(arg1);
    if (hp != NULL) { 		
	    exit(0);
    }
}
