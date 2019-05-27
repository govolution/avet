#pragma once

#include <windows.h>
#include "../debug_print/debug_print.h"


// Hides the console window
// arg1 is not used here and therefore ignored
void hide_console(char *arg1) {
    DEBUG_PRINT("Hiding console window...\n");
    ShowWindow(GetConsoleWindow(), SW_HIDE);
}
