#pragma once

#include "static_from_file.h"


// Static inclusion of data.
// Data is formatted as array and written into the static_data.include file by the set_*_source function.
// This function wraps static_from_file, as the prodecure is the same.
//
// arg1 specifies which array the data is read from.
// data_size receives the size of the data in bytes.
unsigned char *static_from_here(char *arg1, int *data_size) {
    return static_from_file(arg1, data_size);
}
