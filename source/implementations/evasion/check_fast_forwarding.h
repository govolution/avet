#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <time.h>
#include "../debug_print/debug_print.h"

u_int64 Delta(const SYSTEMTIME st1, const SYSTEMTIME st2) {
    union timeunion
    {
        FILETIME fileTime;
        ULARGE_INTEGER ul;
    };

    FILETIME ft1;
    FILETIME ft2;

    SystemTimeToFileTime(&st1, &ft1);
    SystemTimeToFileTime(&st2, &ft2);

    ULARGE_INTEGER u1 = {0};
    ULARGE_INTEGER u2 = {0};

    memcpy(&u1, &ft1, sizeof(u1));
    memcpy(&u2, &ft2, sizeof(u2));

    return u2.QuadPart - u1.QuadPart;
}

// Check if sandbox utilize fast forwarding to reduce heuristic check time
//
// Get time before and after sleep and calculate the difference
// If the difference and specified sleep time match, we proceed
// arg1:        time in Seconds
void check_fast_forwarding(char *arg1) {
    DEBUG_PRINT("Applying check_fast_forwarding technique.\n");

    SYSTEMTIME before_sleep;
    SYSTEMTIME after_sleep;

    GetLocalTime(&before_sleep);

    DEBUG_PRINT("Time before sleep: %d:%d:%d", before_sleep.wHour, before_sleep.wMinute, before_sleep.wSecond);
    DEBUG_PRINT("Sleeping for %s Seconds...\n", arg1);

    int time = atoi(arg1);
    Sleep(time * 1000);

    GetLocalTime(&after_sleep);

    u_int64 i = Delta(before_sleep, after_sleep);

    DEBUG_PRINT("Time difference %s Seconds...\n", i/10000000);

    if (time == i) {
        DEBUG_PRINT("Proceed!\n");
    } else {
        exit(0);
    }
}
