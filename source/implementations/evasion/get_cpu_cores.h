#pragma once

#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include "../debug_print/debug_print.h"


// Investigates the number of CPU cores present on the system. If core count is lower than a specified number, the program exits.
// 
// arg1:        Specifies the core number threshold. Program exits if core count is less than threshold.
void get_cpu_cores(char *arg1) {
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);

    // arg1 is expected to be a valid base-10 integer representation.
    long cpu_core_threshold = strtol(arg1, NULL, 10);

    DEBUG_PRINT("Applying get_cpu_cores evasion technique.\n");
    DEBUG_PRINT("Exiting if number of cpu cores is lower than %ld...\n", cpu_core_threshold);

    if (sysinfo.dwNumberOfProcessors < cpu_core_threshold) {
        exit(0);
    }
}
