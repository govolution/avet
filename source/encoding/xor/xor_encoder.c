#include <stdlib.h>
#include <time.h>
#include "../shellcode_utility.h"


unsigned char generate_random_byte() {
	time_t t;
	srand((unsigned) time(&t));
	return rand() % 256;	
}


// XORs the plaintext with a single-byte key and stores the result in ciphertext
void xor_encode(const unsigned char *plaintext, unsigned char *ciphertext, const int plaintext_length, const unsigned char key) {
	for(int i = 0; i < plaintext_length; i++) {
		ciphertext[i] = plaintext[i] ^ key;
	}
}


// Arguments expected:
// argv[1]: Name of the file containing the shellcode to be encoded
int main(int argc, char **argv) {	
	int shellcode_size;
	// Read shellcode from file into memory
	unsigned char *shellcode =  shellcode_from_file("sc.txt", &shellcode_size);
	
	// Generate random encryption key
	unsigned char key = generate_random_byte();
	
	// Encrypt and overwrite old shellcode file contents with ciphertext
	unsigned char *ciphertext = (unsigned char *) malloc(shellcode_size);
	xor_encode(shellcode, ciphertext, shellcode_size, key);	
	shellcode_to_file(ciphertext, shellcode_size, "scenc.txt");
	
	// Deliver key together with ciphertext in the file
	append_key_to_file(&key, 1, "scenc.txt");		
	
	return 0;
}