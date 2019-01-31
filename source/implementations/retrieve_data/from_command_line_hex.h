#pragma once

#include <string.h>
#include <stdlib.h>
#include "../debug_print/debug_print.h"


// Retrieves data from a "11aabb22..." format hex string given by arg1.
// Can be used to retrieve data of such format from the command line if arg1 = argv[1].
//
// data_size receives the size of the data in bytes.
unsigned char *from_command_line_hex(char *arg1, int *data_size) {
    DEBUG_PRINT("Retrieving data from command line arguments, expecting hex format...\n");
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
