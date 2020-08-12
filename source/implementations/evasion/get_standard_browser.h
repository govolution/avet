#pragma once

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "../debug_print/debug_print.h"


// Programm exits if Browser does not Match with given Argument
//
// arg1:  Programm ID of the Browser: MSEdgeHTM, Firefox or ChromeHTML
void get_standard_browser(char *arg1) {
  char value[255];
  DWORD BufferSize = 8192;

  // Read Registry Key to get the default Browser
  RegGetValue(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\Shell\\Associations\\URLAssociations\\http\\UserChoice", "Progid", RRF_RT_ANY, NULL, (PVOID)&value, &BufferSize);
  DEBUG_PRINT("Applying get_standard_browser evasion technique.\n");
  DEBUG_PRINT("Exiting if provided Browser does not match.\n");

  if(strstr(value, arg1) != NULL){
    DEBUG_PRINT("Proceed!\n");
  } else {
    DEBUG_PRINT("Browsers does not match %s != %s\n", value, arg1);
    exit(0);
  }
}
