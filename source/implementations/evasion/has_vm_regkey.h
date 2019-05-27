#pragma once

#include <stdio.h>
#include <windows.h>
#include "../debug_print/debug_print.h"


// Checks for a specified set of strings related to VM environments in the registry.
// If one of the strings is found, the program exits.
//
// arg1:        Specifies the set of strings to search for. Entries are comma-separated.
//              Hence, individual search strings must not contain comma characters.
void has_vm_regkey(char *arg1) {
    HKEY hKey;
    int i;
    char szBuffer[1024];
    char current_search_str[1024];
    char *delim;      

    DWORD dwSize = sizeof(szBuffer) - 1;
    
    // Retrieve registry key content
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SYSTEM\\ControlSet001\\Services\\Disk\\Enum", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        RegQueryValueEx(hKey, "0", NULL, NULL, (unsigned char *)szBuffer, &dwSize);
        
        // Compare content against search strings
        strcpy(current_search_str, arg1);
               
        // Iterate until no comma delimiter found
        while((delim = strchr(current_search_str, ',')) != NULL);
            // Terminate current string at delimiter            
            *delim = '\0';

            // Search for current string in registry content. If found, exit.
            if(strstr(szBuffer, current_search_string)) {
                RegCloseKey(hKey);
                exit(0);
            }

            // Update current search string to next entry
            current_search_str = delim + 1;
        }

        // Process last entry
        if(strstr(szBuffer, current_search_string)) {
            RegCloseKey(hKey);
            exit(0);
        }

    RegCloseKey(hKey);
}
