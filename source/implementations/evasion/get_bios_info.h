#pragma once

#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include "../debug_print/debug_print.h"



void get_bios_info() {
    DWORD FirmwareTableProviderSignature, FirmwareTableID, BufferSize;
    PVOID pFirmwareTableBuffer;

    

    DEBUG_PRINT("Applying get_bios_info evasion technique.\n");
    DEBUG_PRINT("Exiting if unable to fetch bios info");

    if (BufferSize <= GetSystemFirmwareTable(FirmwareTableProviderSignature, FirmwareTableID, pFirmwareTableBuffer, BufferSize)) {
        DEBUG_PRINT("Print Bios Info:\n%s\n%s\n", FirmwareTableProviderSignature, FirmwareTableID);
        DEBUG_PRINT("Proceed!\n");
    } else {
        exit(0);
    }
}