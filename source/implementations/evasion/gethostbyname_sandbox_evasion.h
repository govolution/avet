#include <stdio.h>
#include <stdlib.h>

// evading with gethostbyname technique
// Link with -lws2_32 switch when compiling
#ifdef PRINT_DEBUG
	printf("use  gethostbyname sandbox evasion\n");
#endif
struct hostent *hp = gethostbyname(KVALUE);
if (hp != NULL) { 		
	exit(0);
}