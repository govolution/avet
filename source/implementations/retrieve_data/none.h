#pragma once


// Dummy function to handle cases where no actual data is retrieved, e.g. no key needed when no encoding is specified.
unsigned char *none(char *arg1, int *data_size)  {
    *data_size = 0;
    return 0;
}
