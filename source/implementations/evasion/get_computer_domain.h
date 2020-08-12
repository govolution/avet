#pragma once

#include <stdio.h>
#include <windows.h>
#include <string.h>
#include "../debug_print/debug_print.h"


// Queries the DNS domain the target is in.
// If the target is not in the expected domain as specified in arg1, the program exits.
//
// arg1:        Expected domain name
void get_computer_domain(char *arg1) {
    DEBUG_PRINT("Applying get_computer_domain sandbox evasion technique.\n");
    DEBUG_PRINT("Expected domain is %s...\n", arg1);

    DWORD bufSize = MAX_PATH;
    TCHAR domainNameBuf[MAX_PATH];

    GetComputerNameEx(ComputerNameDnsDomain, domainNameBuf, &bufSize);

    DEBUG_PRINT("Retrieved domain is %s\n", domainNameBuf);

    if(strcmp(domainNameBuf, arg1) != 0) {
        DEBUG_PRINT("Not in expected domain, exiting...");
        exit(0);
    }
}
