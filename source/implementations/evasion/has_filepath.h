#pragma once

#include <stdio.h>
#include <io.h>
#include "shlwapi.h"
#pragma comment(lib, "Shlwapi.lib")
#include "../debug_print/debug_print.h"

void has_filepath(LPCWSTR arg1) {


    DEBUG_PRINT("Applying has_filepath evasion technique.\n");
    DEBUG_PRINT("Exiting if given filepath does not exist.\n");

    if (PathFileExists(arg1)) {
		DEBUG_PRINT("Proceed!\n")
	} else {
		exit(0)
	}
}
