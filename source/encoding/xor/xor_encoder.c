#include <stdlib.h>


// single-byte XOR
unsigned char *xor_encode(unsigned char *plaintext, int plaintext_length, unsigned char key) {
	unsigned char *ciphertext = (unsigned char *) malloc(plaintext_length);
	
	for(int i = 0; i < plaintext_length; i++) {
		ciphertext[i] = plaintext[i] ^ key;
	}
	
	return ciphertext;
}


// Arguments expected:
// argv[1]: Name of the shellcode file to be manipulated
int main(int argc, char **argv) {
	
	
	
	return 0;
}