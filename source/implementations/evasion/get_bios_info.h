#pragma once

#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include "../debug_print/debug_print.h"

struct RawSMBIOSData
{
    BYTE    Used20CallingMethod;
    BYTE    SMBIOSMajorVersion;
    BYTE    SMBIOSMinorVersion;
    BYTE    DmiRevision;
    DWORD    Length;
    BYTE    SMBIOSTableData[];
};

// Checks if it is possible to fetch SMBIOS firmware table
//
// Proceed if possible, else exit.
void get_bios_info(char *arg1) {
  DWORD smBiosDataSize = 0;
  struct RawSMBIOSData *smBiosData = NULL;
  DWORD bytesWritten = 0;

  DEBUG_PRINT("Applying get_bios_info evasion technique.\n");
  DEBUG_PRINT("Exiting if unable to fetch bios info\n");

  // Query size of SMBIOS data.
  smBiosDataSize = GetSystemFirmwareTable('RSMB', 0, NULL, 0);

  smBiosData = (struct RawSMBIOSData*) HeapAlloc(GetProcessHeap(), 0, smBiosDataSize);
  if (!smBiosData) {
    DEBUG_PRINT("Out of memory\n");
  }

  // Retrieve the SMBIOS table
  bytesWritten = GetSystemFirmwareTable('RSMB', 0, smBiosData, smBiosDataSize);

  if (bytesWritten == smBiosDataSize) {
    DEBUG_PRINT("Proceed!\n");
  } else {
    exit(0);
  }
}
