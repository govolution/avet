#pragma once

#include <stdio.h>
#include <string.h>
#include "../debug_print/debug_print.h"


// Currently ignores payload_info.
// Currently also ignores shellcode_size, as the function binding does not require it.
void exec_shellcode(unsigned char *shellcode, int shellcode_size, char *payload_info) {
	DEBUG_PRINT("exec_shellcode called.\n");
	
    // Check for NULL pointer to handle cases where no shellcode data was retrieved
    if(shellcode != NULL) {
	    int (*funct)();
	    funct = (int (*)()) shellcode;
	    (int)(*funct)();
    }
}
