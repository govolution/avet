#pragma once

#include <stdio.h>
#include <windows.h>
#include "../debug_print/debug_print.h"


// Checks for a specified set of strings related to VM environments in the registry.
// If one of the strings is found, the program exits.
//
// arg1:        Specifies the set of strings to search for. Entries are comma-separated.
//              Hence, individual search strings must not contain comma characters.
//              Total search string length is currently limited to 1023.
void has_vm_regkey(char *arg1) {
    DEBUG_PRINT("Applying has_vm_regkey evasion technique.\n");
    DEBUG_PRINT("Checking registry key SYSTEM\\ControlSet001\\Services\\Disk\\Enum for search set %s...\n", arg1);

    HKEY hKey;
    int i;
    char szBuffer[1024];
    char complete_search_str[1024];
    char *current_search_str;
    char *delim;      

    DWORD dwSize = sizeof(szBuffer) - 1;
    
    // Retrieve registry key content
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SYSTEM\\ControlSet001\\Services\\Disk\\Enum", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        RegQueryValueEx(hKey, "0", NULL, NULL, (unsigned char *)szBuffer, &dwSize);
        
        // Compare content against search strings
        // Work on our own copy
        strcpy(complete_search_str, arg1);
        // Set current marker at beginning               
        current_search_str = complete_search_str;

        // Iterate until no comma delimiter found
        while((delim = strchr(current_search_str, ',')) != NULL) {
            // Terminate current string at delimiter            
            *delim = '\0';

            // Search for current string in registry content. If found, exit.
            if(strstr(szBuffer, current_search_str)) {
                DEBUG_PRINT("Found %s, exiting...\n", current_search_str);                
                RegCloseKey(hKey);
                exit(0);
            }

            // Update current search string to next entry
            current_search_str = delim + 1;
        }

        // Process last entry
        if(strstr(szBuffer, current_search_str)) {
            DEBUG_PRINT("Found %s, exiting...\n", current_search_str);            
            RegCloseKey(hKey);
            exit(0);
        }
        
        RegCloseKey(hKey);
    } else {
        DEBUG_PRINT("Could not open registry key.\n");
    }

    DEBUG_PRINT("Found none. Proceeding.\n");
}
