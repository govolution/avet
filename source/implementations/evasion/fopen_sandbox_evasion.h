#pragma once

#include <stdio.h>
#include <stdlib.h>


// Argument arg1 is not used and therefore ignored.
void fopen_sandbox_evasion(char *arg1) {
    // do evading here with fopen technique
    #ifdef PRINT_DEBUG
	    printf("use fopen sandbox escape\n");
    #endif
    FILE *fp = fopen("c:\\windows\\system.ini", "rb");
    if (fp == NULL) {
	    exit(0);	
    }
    fclose(fp);
}
