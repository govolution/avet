#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "../debug_print/debug_print.h"


// Checks if a given file exists on the system. If opening fails, the program exits.
//
// arg1:        Specifies the file to check upon.
void fopen_sandbox_evasion(char *arg1) {
    DEBUG_PRINT("Applying fopen sandbox evasion technique.\n");
    DEBUG_PRINT("Checking for file %s...\n", arg1);
    
    FILE *fp = fopen(arg1, "rb");

    if (fp == NULL) {
	    exit(0);	
    }

    fclose(fp);
}
