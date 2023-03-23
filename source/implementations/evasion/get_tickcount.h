#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "../debug_print/debug_print.h"
#include <string.h>

// Check if sandbox utilize fast forwarding to reduce heuristic check time
//
// use GetTickCount before and after sleep and calculate the difference
// If the difference and specified sleep time match, we proceed
// arg1:        nothing
void get_tickcount(char *arg1) {
    DEBUG_PRINT("Applying get_tickcount technique.\n");

    ULONGLONG uptimeBeforeSleep = GetTickCount();

    LARGE_INTEGER delay;
    delay.QuadPart = -10000 * 100000; // 100 seconds
    typedef NTSTATUS(WINAPI *PNtDelayExecution)(IN BOOLEAN, IN PLARGE_INTEGER);
    PNtDelayExecution pNtDelayExecution = (PNtDelayExecution)GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "NtDelayExecution");

    pNtDelayExecution(FALSE, &delay);

    ULONGLONG uptimeAfterSleep = GetTickCount();

    if ((uptimeAfterSleep - uptimeBeforeSleep) < 100000) {
        exit(0);
    } else {
        DEBUG_PRINT("Proceed!\n");
    }
}
