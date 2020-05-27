#pragma once

#include <stdio.h>
#include <string.h>
#include <windows>
#include "../debug_print/debug_print.h"

void has_unsername(char *arg1) {

    char* username[100];

    username = getenv("USERNAME");

    DEBUG_PRINT("Applying has_username evasion technique.\n");
    DEBUG_PRINT("Exiting if given username does not match.\n");

    if (strcmp(username, arg1)) {
		DEBUG_PRINT("Proceed!\n")
	} else {
		exit(0)
	}
}