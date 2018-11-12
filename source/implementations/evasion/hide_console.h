#pragma once

#include <windows.h>


// Hides the console window
// arg1 is not used and therefore ignored
void hide_console(char *arg1) {
    ShowWindow(GetConsoleWindow(), SW_HIDE);
}
