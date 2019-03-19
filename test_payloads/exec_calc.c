#include <stdlib.h>
#include <windows.h>


BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    system("calc.exe");
    return TRUE;
}
