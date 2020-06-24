#pragma once

#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <stdlib.h>
#include "../debug_print/debug_print.h"


// Lookup the username of the current user
//
// arg1: username, if given username does not match with the fetched username
// the Program exits
void has_username(char *arg1) {

  char* username = getenv("USERNAME");;


  DEBUG_PRINT("Applying has_username evasion technique.\n");
  DEBUG_PRINT("Exiting if given username does not match.\n");

  if (!strcmp(username, arg1)) {
    DEBUG_PRINT("Proceed!\n");
  } else {
    DEBUG_PRINT("Username does not match %s != %s.\n", arg1, username);
    exit(0);
  }
}
