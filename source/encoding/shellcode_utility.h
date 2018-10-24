#include <stdio.h>


// Writes the given shellcode to the specified file in a format that AVET expects as input.
// ------
// THE PREVIOUS CONTENTS OF THE FILE WILL BE OVERWRITTEN!
// ------
// The output consists of an array unsigned char buf[], which contains the shellcode.
// The output file can be directly included in AVET sources to get shellcode access.
// shellcode_size is counted in bytes.
void shellcode_to_file(unsigned char *shellcode, int shellcode_size, char *filename) {
	// Remove previous file contents
	FILE *shellcode_file = fopen(filename, "w");
	fclose(shellcode_file);
	
	// Write shellcode to file
	shellcode_file = fopen(filename, "a");
		
	fputs("\n");
	fputs("unsigned char buf[] = \"");
	
	for(int i = 0; i < shellcode_size; i++) {
		fputs("\\x%x", shellcode[i]);
	}
	
	fputs("\";");
	fputs("\n");
	fclose(shellcode_file);
}


unsigned char *shellcode_from_file(char *filename) {
		
}

