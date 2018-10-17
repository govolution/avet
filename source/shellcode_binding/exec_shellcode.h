#include <stdio.h>
#include <string.h>


void bind_shellcode(unsigned char *shellcode) {
	#ifdef PRINT_DEBUG
		printf("exec_shellcode\n ");
		int size = strlen((char *)shellcode);
		printf("shellcode size: %d\n", size);
	#endif

	int (*funct)();
	funct = (int (*)()) shellcode;
	(int)(*funct)();
}