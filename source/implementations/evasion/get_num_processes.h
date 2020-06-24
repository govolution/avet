#include <windows.h>
#include <stdio.h>
#include <psapi.h>
#include "../debug_print/debug_print.h"


// Count the number of running Processes
//
// arg1:    Specifies process threshold. Program exits if process count is less than threshold.
void get_num_processes(char *arg1) {
  int minNumProcesses = atoi(arg1);

  DWORD loadedProcesses[1024];
	DWORD cbNeeded;
	DWORD runningProcesses;

  if (!EnumProcesses(loadedProcesses, sizeof(loadedProcesses), &cbNeeded)) {
	   DEBUG_PRINT("Could not get processes.\n");
	}

  runningProcesses = cbNeeded / sizeof(DWORD);

  DEBUG_PRINT("Applying get_num_processes evasion technique.\n");
  DEBUG_PRINT("Exiting if number of processes are below threshold.\n");

  if (runningProcesses >= minNumProcesses) {
    DEBUG_PRINT("Proceed!.\n");
  } else {
    exit(0);
  }
}
