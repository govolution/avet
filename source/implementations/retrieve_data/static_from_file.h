#pragma once

#include "../../debug_print/debug_print.h"


// Static retrieval of data from file.
// Static in this context means that the data is statically compiled into the executable and therefore fixed after compilation time.
// Data must be copied as c-style array buf[] into get_shellcode.h by the build script.
//
// arg1 serves no purpose here.
// data_size receives the size of the data in bytes.
unsigned char *static_from_file(char *arg1, int *data_size) {
    DEBUG_PRINT(("Statically retrieving data from array buf[] in included file...\n"));
    *data_size = sizeof(buf) - 1;	
    return buf;
}
