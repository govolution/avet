#pragma once

#include <stdio.h>
#include "../debug_print/debug_print.h"


void exec_shellcode_ASCIIMSF(unsigned char *shellcode) {
	DEBUG_PRINT(("exec_shellcode_ASCIIMSF called\n"));
	int size=strlen(shellcode);
	DEBUG_PRINT(("shellcode size: %d\n", size));
	
    // Check for NULL pointer to handle cases where no shellcode data was retrieved
    if(shellcode != NULL) {
	    register unsigned char* r asm("eax");
	    r=shellcode;
	    asm("call *%eax;");
    }
}
