#include "shellcode_utility.h"


// Takes raw shellcode as input from a file, converts it into C-array format and writes output to another file.
//
// argv[1]:     Name of the file to read the raw shellcode from
// argv[2]:     Name of the file to write the C-style shellcode to
int main(int argc, char **argv) {
    int shellcode_size = 0;
    unsigned char *shellcode = shellcode_from_file_raw(argv[1], &shellcode_size);
    shellcode_to_file(shellcode, shellcode_size, argv[2]);
    return 0;
}
