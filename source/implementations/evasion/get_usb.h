#pragma once

#include <windows.h>
#include <stdio.h>
#include "../debug_print/debug_print.h"


//
// WIP
//
void get_usb(char *arg1) {
    HKEY hKey;
    DWORD numUsbDevices = 0;
    int MinimumUsbHistory = atoi(arg1);

    DEBUG_PRINT("Applying get_usb evasion technique.\n");

    // Open Key with USB Information
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\ControlSet001\\Enum\\USBSTOR", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        // number of Subkeys = number of mounted USB devices
        // Get number of subkeys
        if (RegQueryInfoKeyA(hKey, NULL, NULL, NULL, &numUsbDevices, NULL, NULL, NULL, NULL, NULL, NULL, NULL) == ERROR_SUCCESS) {
        
        } else {
            DEBUG_PRINT("Unable to query subkey HKLM::SYSTEM\\ControlSet001\\Enum\\USBSTOR\n");
        }
    } else {
		DEBUG_PRINT("Unable to open subkey HKLM::SYSTEM\\ControlSet001\\Enum\\USBSTOR\n");
	}

    DEBUG_PRINT("Exiting if number of mounted usb devices is lower than %ld...\n", MinimumUsbHistory);

    if (numUsbDevices >= MinimumUsbHistory) {
		DEBUG_PRINT("Number of USB devices ever mounted: %d\n", numUsbDevices);
		DEBUG_PRINT("Proceed!\n");
	} else {
		exit(0);
    }
}