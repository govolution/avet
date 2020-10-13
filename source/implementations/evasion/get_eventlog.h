#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../debug_print/debug_print.h"


// Investigates the number of eventlogs present on the system. If eventlog count is lower than a specified number, the program exits.
// 
// arg1:        Specifies the eventlog number threshold. Program exits if eventlog count is less than threshold.
void get_eventlog(char *arg1) {
    DEBUG_PRINT("Applying get_eventlog evasion technique.\n");
  
    char *call = "powershell.exe ";
    char *pwsh = "$lines = Get-WinEvent -ListLog * ^| measure-object -line; echo $lines.Lines";
    char *command = (char *) malloc(strlen(pwsh) + strlen(call) + 1);
    long num_eventlog_threshold = strtol(arg1, NULL, 10);

    FILE *fp;
    char buf[128];
    char retval[10];

    strcpy(command, call);
    strcat(command, pwsh);

    if ((fp = popen(command, "r")) == NULL) {
        DEBUG_PRINT("Error opening pipe!\n");
        exit(0);
    }

    while (fgets(buf, 128, fp) != NULL) {
        strcat(retval, buf);
    }
    
    long eventlog_count = strtol(retval, NULL, 10);
    DEBUG_PRINT("%d\n", eventlog_count);

    if (eventlog_count > num_eventlog_threshold) {
        DEBUG_PRINT("Proceed!\n");
    } else {
        exit(0);
    }
}