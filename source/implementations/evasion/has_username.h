#pragma once

#include <stdio.h>
#include <windows.h>
#include "../debug_print/debug_print.h"

void has_unsername(char *arg1) {
    WCHAR* username[3267];
	DWORD charCount[3267];

    if (!GetUserName(username, charCount)) {
        DEBUG_PRINT("Could not read username.\n");
        getchar();
    }

    DEBUG_PRINT("Applying has_username evasion technique.\n");
    DEBUG_PRINT("Exiting if given username does not match.\n");

    if (!wcscmp(username, argv[1])) {
		DEBUG_PRINT("Proceed!\n")
	} else {
		exit(0)
	}
}