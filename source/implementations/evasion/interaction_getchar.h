#pragma once

#include <stdio.h>
#include "../debug_print/debug_print.h"


// Wait until input is delivered using getchar.
//
// arg1:        not used
void interaction_getchar(char *arg1) {
    DEBUG_PRINT("Waiting for input...\n");
    char input = getchar();
    putchar(input);
    DEBUG_PRINT("Input delivered, proceeding...");
}
