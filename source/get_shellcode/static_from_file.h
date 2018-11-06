// Static retrieval of shellcode from file.
// Static in this context means that the shellcode is statically compiled into the executable and therefore fixed after compilation time.
// Shellcode must be copied as c-style array buf[] into get_shellcode.h by the build script.
//
// arg1 serves no purpose here.
// shellcode_size receives the size of the shellcode in bytes.
unsigned char *get_shellcode(char *arg1, int *shellcode_size) {
    *shellcode_size = sizeof(buf) - 1;	
    return buf;
}
