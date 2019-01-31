#pragma once

#include <stdio.h>
#include "../debug_print/debug_print.h"
#include "helper_functions/helper_functions.h"


// Dynamic retrieval of data from file
// Dynamic in this context means that the data is not statically compiled into the executable,
// but instead is retrieved from a file at run time.
//
// arg1 specifies the filename to fetch the data from.
// data_size receives the size of the data in bytes.
unsigned char *dynamic_from_file(char *arg1, int *data_size) {	
	// First command line argument specifies the file name to read from
	char *filename = arg1;	
	
	DEBUG_PRINT("Dynamically retrieving data from file %s...\n", filename);
	
	*data_size = get_filesize(filename);
	// Memory allocation is handled by load_textfile
	return load_textfile(filename, *data_size);		
}
