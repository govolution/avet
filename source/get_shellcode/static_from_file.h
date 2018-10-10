// Static retrieval of shellcode from file
// Static in this context means that the shellcode is statically compiled into the executable and therefore fixed after compilation time
// Shellcode is copied into array buf[] in get_shellcode.h by the build script
unsigned char *get_shellcode(char *arg1) {
	return buf;
}