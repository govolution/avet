#pragma once

#include <string.h>
#include <stdlib.h>


// Retrieves the shellcode from a "11aabb22..." format hex string given by arg1.
// Can be used to retrieve shellcode of such format from the command line if arg1 = argv[1].
//
// shellcode_size receives the size of the shellcode in bytes.
unsigned char *get_shellcode(char *arg1, int *shellcode_size) {
    // Get input string length  
    int input_length = strlen(arg1);

    // Convert ASCII hex string into raw bytes
    char current_hex[5] = "0xaa";

    // Each shellcode byte is represented by 2 ASCII symbols
    *shellcode_size = input_length / 2;
    unsigned char *shellcode = (unsigned char *) malloc(shellcode_size);

    // Hex string is of format aabb01cc34...
	// Each loop iteration handles a 2-character chunk
    int j = 0;
    char *endptr;						// Required by strtuol function

	for(int i = 0; i < input_length; i++) {
		// Interpret hex representation as unsigned char value 
		memcpy(&(current_hex[2]), &(shellcode_string[2 * i]), 2);		
		shellcode[j] = (unsigned char) strtoul(current_hex, &endptr, 16);
		j++;
	}   

    return shellcode;     
}
