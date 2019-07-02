#include <stdio.h>
#include <stdlib.h>
#include "../../../data_utility.h"
#include "rc4.h"


// Arguments expected:
// argv[1]: Name of the file containing the payload to be encoded (raw format expected)
// argv[2]: Name of the file where the encoded payload shall be written to
// argv[3]: Name of the file the key to be applied is stored (raw format)
int main(int argc, char **argv) {	
	int payload_size;
    int key_length;	

    printf("Starting RC4 encoder...\n");
    
	// Read payload from file into memory
    printf("Reading payload from file %s, expecting raw format.\n", argv[1]);	
    unsigned char *payload = data_from_file_raw(argv[1], &payload_size);
    printf("payload size in bytes is %d\n", payload_size);

    // Read encryption key from file into memory
    printf("Reading key from file %s, expecting raw format.\n", argv[3]);
    unsigned char *key = data_from_file_raw(argv[3], &key_length);
    printf("Key length in bytes is %d\n", key_length);

    // Encrypt and write ciphertext to file
	unsigned char *ciphertext = (unsigned char *) malloc(payload_size);
    printf("Applying RC4 algorithm\n");
	RC4(payload, payload_size, key, key_length, ciphertext);	
    printf("Writing payload to file %s\n", argv[2]);
	data_to_file_raw(ciphertext, payload_size, argv[2]);
	
	return 0;
}
