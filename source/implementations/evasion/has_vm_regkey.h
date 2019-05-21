#pragma once

#include <stdio.h>
#include <windows.h>
#include "../debug_print/debug_print.h"


// Argument arg1 is not used and therefore ignored.
void has_vm_regkey(char *arg1)
{
    HKEY hKey;
    int i;
    char szBuffer[1024];
    char *szProducts[] = { "VMWARE", "VBOX", "VIRTUAL", "VMware" };
    DWORD dwSize = sizeof(szBuffer) - 1;

    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SYSTEM\\ControlSet001\\Services\\Disk\\Enum", 0, KEY_READ, &hKey) == ERROR_SUCCESS)
    {
        RegQueryValueEx(hKey, "0", NULL, NULL, (unsigned char *)szBuffer, &dwSize );
        for (i = 0; i < _countof(szProducts); i++)
        {
            if (strstr(szBuffer, szProducts[i]))
            {
                RegCloseKey(hKey);
                exit(0);
            }
        }
    }

    RegCloseKey(hKey);
}

