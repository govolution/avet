#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../debug_print/debug_print.h"


void get_install_date(char *arg1) {
  char *cmd = "systeminfo | find /i \"original\"";
  FILE *fp;
  char installdate[200];

  if ((fp = popen(cmd, "r")) == NULL) {
        DEBUG_PRINT("Error opening pipe!\n");
        exit(0);
  }

  while (fgets(buf, 128, fp) != NULL) {
      strcat(installdate, buf);
  }

  DEBUG_PRINT("Applying get_install_date evasion technique.\n");
  DEBUG_PRINT("Exiting if dates does not match.\n");


  if (strstr(installdate, arg1) != NULL) {
    DEBUG_PRINT("Proceed!\n");

  } else {
    DEBUG_PRINT("Dates does not match: %s != %s", installdate, arg1);
    exit(0);
  }
}
