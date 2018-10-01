#include <stdio.h>
#include <stdlib.h>

// WARNING: Does not link gethostbyname() properly
// evading with gethostbyname technique
#ifdef PRINT_DEBUG
	printf("use  gethostbyname sandbox evasion\n");
#endif
struct hostent *hp = gethostbyname(KVALUE);
if (hp != NULL) { 		
	exit(0);
}