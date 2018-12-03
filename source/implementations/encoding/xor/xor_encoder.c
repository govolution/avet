#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../../../data_utility.h"


// Generates a (non-cryptographically) random byte sequence.
// Length of the sequence will be key_length bytes.
// Memory for the key will be allocated by the function.
unsigned char *generate_key(int key_length) {
    // Init random structures	
    time_t t;
	srand((unsigned) time(&t));

    // Allocate memory for key
    unsigned char *key = (unsigned char *) malloc(key_length);
    
    // Generate key bytes
    for(int i = 0; i < key_length; i++) {
        key[i] = rand() % 256;    
    }

	return key;	
}


// XORs the plaintext with key bytes and stores the result in ciphertext.
// Can handle multi-byte keys, key bytes are then applied successively.
// Plaintext length is not required to be a multiple of the key length, the procedure just stops when the plaintext ends.
void xor_encode(const unsigned char *plaintext, const int plaintext_length, const unsigned char *key, const int key_length, unsigned char *ciphertext) {
    for(int i = 0; i < plaintext_length; i++) {
		ciphertext[i] = plaintext[i] ^ key[i % key_length];
	}
}


// Arguments expected:
// argv[1]: Name of the file containing the shellcode to be encoded (raw format expected)
// argv[2]: Name of the file where the encoded shellcode shall be written to
// argv[3]: Length of the key to be applied, in bytes. Specify as decimal number.
// argv[4]: Name of the file where the used crypto key shall be written to
int main(int argc, char **argv) {	
	int shellcode_size;
	
    printf("Starting XOR encoder...\n");
    
	// Read shellcode from file into memory
    printf("Reading shellcode from file %s, expecting raw format.\n", argv[1]);	
    unsigned char *shellcode =  data_from_file_raw(argv[1], &shellcode_size);
    printf("Shellcode size in bytes is %d\n", shellcode_size);

    // Convert key length
    int key_length = strtol(argv[3], NULL, 10);    

	// Generate random encryption key
    printf("Generating %d byte key:\t", key_length);
    unsigned char *key = generate_key(key_length);
    for(int i = 0; i < key_length; i++) {
        printf("%02x", key[i]);     
    }
    printf("\n");    

	// Encrypt and write ciphertext to file
	unsigned char *ciphertext = (unsigned char *) malloc(shellcode_size);
    printf("Applying XOR algorithm\n");
	xor_encode(shellcode, shellcode_size, key, key_length, ciphertext);	
    printf("Writing shellcode to file %s\n", argv[2]);
	data_to_file_raw(ciphertext, shellcode_size, argv[2]);
	
	// Deliver key in another output file
    printf("Writing key to file %s\n", argv[4]);
	data_to_file_raw(key, 1, argv[4]);		
	
	return 0;
}
