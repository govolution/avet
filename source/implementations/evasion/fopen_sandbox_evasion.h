#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "../debug_print/debug_print.h"


// Argument arg1 is not used and therefore ignored.
void fopen_sandbox_evasion(char *arg1) {
    // do evading here with fopen technique
    DEBUG_PRINT("Using fopen sandbox evasion technique...\n");
    
    FILE *fp = fopen("c:\\windows\\system.ini", "rb");
    if (fp == NULL) {
	    exit(0);	
    }
    fclose(fp);
}
