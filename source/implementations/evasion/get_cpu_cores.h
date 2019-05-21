#pragma once

#include <stdio.h>
#include <windows.h>
#include "../debug_print/debug_print.h"

// Argument arg1 is not used and therefore ignored.
void get_cpu_cores(char *arg1) {
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    if (sysinfo.dwNumberOfProcessors >= MIN_CPU_CORES)
	exit(0);
}
