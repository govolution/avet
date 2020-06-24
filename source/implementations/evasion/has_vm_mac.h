/* Large parts of the code copied/inspired from https://github.com/a0rtega/pafish
   should work for vmware and VirtualBox */
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <windows.h>
#include <iphlpapi.h>
#include "../debug_print/debug_print.h"


/* left the original pafish function */
int pafish_check_mac_vendor(char * mac_vendor) {
	unsigned long alist_size = 0, ret;

	ret = GetAdaptersAddresses(AF_UNSPEC,0,0,0,&alist_size);
	if(ret==ERROR_BUFFER_OVERFLOW) {
		IP_ADAPTER_ADDRESSES* palist = (IP_ADAPTER_ADDRESSES*)LocalAlloc(LMEM_ZEROINIT,alist_size);
		void * palist_free = palist;
		if(palist) {
			GetAdaptersAddresses(AF_UNSPEC,0,0,palist,&alist_size);
			char mac[6]={0};
			while (palist){
				if (palist->PhysicalAddressLength==0x6){
					memcpy(mac,palist->PhysicalAddress,0x6);
					if (!memcmp(mac_vendor, mac, 3)) { /* First 3 bytes are the same */
						LocalFree(palist_free);
						return TRUE;
					}
				}
				palist = palist->Next;
			}
			LocalFree(palist_free);
		}
	}
	return FALSE;
}


void has_vm_mac()
{
    /*
		VMware is any of
		00:05:69
		00:0C:29
		00:1C:14
		00:50:56
	*/
	if (pafish_check_mac_vendor("\x00\x05\x69")) {
        DEBUG_PRINT("MAC VMWare found.\n");
		exit(0);
    }
	else if (pafish_check_mac_vendor("\x00\x0C\x29")) {
        DEBUG_PRINT("MAC VMWare found.\n");
		exit(0);
	}
	else if (pafish_check_mac_vendor("\x00\x1C\x14")) {
        DEBUG_PRINT("MAC VMWare found.\n");
		exit(0);
	}
	else if (pafish_check_mac_vendor("\x00\x50\x56")) {
        DEBUG_PRINT("MAC VMWare found.\n");
		exit(0);
	}
	/* VirtualBox mac starts with 08:00:27 */
	else if (pafish_check_mac_vendor("\x08\x00\x27")) {
        DEBUG_PRINT("MAC VirtualBox found.\n");
		exit(0);
	}

}
