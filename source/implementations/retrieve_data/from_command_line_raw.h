#pragma once

#include <string.h>
#include <stdlib.h>
#include "../debug_print/debug_print.h"


// Retrieved data from the command line.
// The given ASCII string is interpreted as raw byte data.
// Can be used to retrieve data of such format from the command line if arg1 = argv[1].
//
// data_size receives the size of the data in bytes.
unsigned char *from_command_line_raw(char *arg1, int *data_size) {
    DEBUG_PRINT("Retrieving data from command line arguments, expecting raw ASCII format...\n");
    // Get input string length  
    *data_size = strlen(arg1);

    // Interpret as raw data and copy into new allocated buffer
    unsigned char *data = (unsigned char *) malloc(*data_size);
    memcpy(data, arg1, *data_size);
   
    return data;     
}
