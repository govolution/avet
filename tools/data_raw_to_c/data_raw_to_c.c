#include "../../source/data_utility.h"


// Takes raw data as input from a file, converts it into C-array format and writes output to another file.
//
// argv[1]:     Name of the file to read the raw data from
// argv[2]:     Name of the file to write the C-style array into
// argv[3]:     Name of the array to write the data into
int main(int argc, char **argv) {
    int data_size = 0;
    unsigned char *data = data_from_file_raw(argv[1], &data_size);
    data_to_file(data, data_size, argv[2], argv[3]);
    return 0;
}
