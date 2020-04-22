#pragma once

#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "../debug_print/debug_print.h"
#include "helper_functions/helper_functions.h"

// Downloads Data with Curl
// The data is then read from the file and returned.
//
// data_size receives the size of the data in bytes.

unsigned char *download_curl(char *arg1, int *data_size) {
	DEBUG_PRINT("Downloading data to file via curl...\n");
	 
    char sh_filename[128];
    strcpy(sh_filename, get_filename_from_url(arg1));
    
    DEBUG_PRINT("sh_filename = %s\n", sh_filename);
    
    char download[500];
    sprintf(download, "curl.exe %s -o %s", arg1, sh_filename);
    
    DEBUG_PRINT("command: %s\n", download);
    system(download);
    DEBUG_PRINT("Download done.\n");
    
    *data_size = get_filesize(sh_filename);
    return load_textfile(sh_filename, *data_size);
}
