#include <stdio.h>
#include <stdlib.h>


// do evading here with fopen technique
#ifdef PRINT_DEBUG
	printf("use fopen sandbox escape\n");
#endif
FILE *fp = fopen("c:\\windows\\system.ini", "rb");
if (fp == NULL) {
	exit(0);	
}
fclose(fp);
	