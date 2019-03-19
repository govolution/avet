#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../../source/data_utility.h"


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


// Retrieves data from a "11aabb22..." format hex string given by arg1.
//
// data_size receives the size of the data in bytes.
unsigned char *from_command_line_hex(char *arg1, int *data_size) {
    // Get input string length  
    int input_length = strlen(arg1);

    // Convert ASCII hex string into raw bytes
    char current_hex[5] = "0xaa";

    // Each data byte is represented by 2 ASCII symbols
    *data_size = input_length / 2;
    unsigned char *data = (unsigned char *) malloc(*data_size);

    // Hex string is of format aabb01cc34...
	// Each loop iteration handles a 2-character chunk
    int j = 0;
    char *endptr;						// Required by strtoul function

	for(int i = 0; i < input_length; i++) {
		// Interpret hex representation as unsigned char value 
		memcpy(&(current_hex[2]), &(arg1[2 * i]), 2);		
		data[j] = (unsigned char) strtoul(current_hex, &endptr, 16);
		j++;
	}   

    return data;     
}


// Key generation utility. Generates either a random key or takes a preset key,
// and outputs the raw (!!!) key data into a specified file.
// Arguments expected:
//
// argv[1]: (random|preset)
//      Random: Generates a random byte sequence. Sequence length in bytes is specified in argv[2].
//      Preset: Allows to enter a preset key via argv[2]. Input format is a hex string, such as aabbccdd...
// argv[2]: Either specifies key length in bytes, or a preset key via hex string, depending on argv[1] choice.
// argv[3]: Name of the output file to write generated raw key data into. 

int main(int argc, char **argv) {
    int key_length = 0;    
    unsigned char *key = 0;    

    // Generate random key
    if(strcmp(argv[1], "random") == 0) {
        // Convert key length specified in second argument
        key_length = strtol(argv[2], NULL, 10);
        // Generate key of specified length
        key = generate_key(key_length);        
    }

    // Read preset key
    if(strcmp(argv[1], "preset") == 0) {
        // Get key from command line argument argv[2]
        key = from_command_line_hex(argv[2], &key_length);           
    }

    // Write raw key data to output file
    data_to_file_raw(key, key_length, argv[3]);

    return 0;
}
