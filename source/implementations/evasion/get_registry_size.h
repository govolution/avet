#pragma once

#include <stdio.h>
#include <windows.h>
#include "../debug_print/debug_print.h"

//
//  WIP
//
void get_registry_size(char *arg1) {
  DWORD BufferSize = 8192;
  DWORD cbData;
  DWORD dwRet;

  long threshold = strtol(arg1, NULL, 10);

  PPERF_DATA_BLOCK PerfData = (PPERF_DATA_BLOCK) malloc( BufferSize );
  cbData = BufferSize;

  DEBUG_PRINT("Applying get_registry_size evasion technique.\n");
  DEBUG_PRINT("Exiting if size is less than given threshold\n");

  dwRet = RegQueryValueEx( HKEY_PERFORMANCE_DATA,
                           TEXT("Global"),
                           NULL,
                           NULL,
                           (LPBYTE) PerfData,
                           &cbData );

  if (dwRet == ERROR_SUCCESS && BufferSize >= threshold) {
    DEBUG_PRINT("Proceed!\n");
  } else {
    exit(0);
  }
}
