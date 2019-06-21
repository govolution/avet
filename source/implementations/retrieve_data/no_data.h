#pragma once

#include "../debug_print/debug_print.h"


// Dummy function to handle cases where no actual data is retrieved, e.g. no key needed when no encoding is specified.
unsigned char *no_data(char *arg1, int *data_size)  {
    DEBUG_PRINT("\"no_data\" retrieve_data function called.\n"); 
    *data_size = 0;
    return 0;
}
