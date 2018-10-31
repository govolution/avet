#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Internal helper function.
// Removes all occurences of character chr in string str and overwrites str with result.
static void remove_all_chars(char *str, const char chr) {
	// Compute input string length
	int str_len = strlen(str);
	
	// Allocate memory for result string	
	char *result = (char *) malloc(str_len + 1);
	
	int j = 0;	// Index for result string
	
	for(int i = 0; i < str_len; i++) {
		// Copy character to result if != chr
		if(str[i] != chr) {
			result[j] = str[i];
			j++;
		}
	}
	
	// Terminate result
	result[j] = '\0';
	
	// Copy result
	strcpy(str, result);
	
	// Free result string
	free(result);
}


// Writes the given shellcode to the specified file in a format that AVET expects as input.
// ------
// THE PREVIOUS CONTENTS OF THE FILE WILL BE OVERWRITTEN!
// ------
// The output consists of an array unsigned char buf[], which contains the shellcode.
// The output file can be directly included in AVET sources to get shellcode access.
// shellcode_size is counted in bytes.
void shellcode_to_file(const unsigned char *shellcode, const int shellcode_size, const char *filename) {
	// Remove previous file contents
	FILE *shellcode_file = fopen(filename, "w");
	fclose(shellcode_file);
	
	// Write shellcode to file
	shellcode_file = fopen(filename, "a");
		
	fputs("\n", shellcode_file);
	fputs("unsigned char buf[] = \"", shellcode_file);
	
	char current_hex[4];
	
	for(int i = 0; i < shellcode_size; i++) {
		sprintf(current_hex, "\\x%02x", shellcode[i]);
		fputs(current_hex, shellcode_file);
	}
	
	fputs("\";", shellcode_file);
	fputs("\n", shellcode_file);
	fclose(shellcode_file);
}


// Reads shellcode from a file, given in format unsigned char buf[] = "\xaa\xbb\xcc...");
// Returns pointer to the shellcode read into memory. Required memory will be allocated by the function.
// shellcode_size will be filled with the size of the shellcode in bytes.
unsigned char *shellcode_from_file(const char *filename, int *shellcode_size) {		
	// Assume that file only contains array unsigned char buf[] = "\xaa\xbb\xcc...";
	FILE *shellcode_file = fopen(filename, "r");
	
	// Get file size
	fseek(shellcode_file, 0L, SEEK_END);
	int file_size = ftell(shellcode_file);
	rewind(shellcode_file);	
	
	// Allocate memory for string
	char *file_content = (char *) malloc(file_size + 1);
				
	// Get whole file content as string
	fread(file_content, 1, file_size, shellcode_file);
	file_content[file_size] = '\0';
	
	// Close file
	fclose(shellcode_file);	
	
	// Search for first occurence of backslash character (marks beginning of shellcode string)
	char *shellcode_string = strchr(file_content, '\\');	
	
	// Format shellcode string to make parsing easier:
	// Search for array end ';' and terminate string after that character
	strchr(shellcode_string, ';')[1] = '\0';	
	// Remove all linebreaks
	remove_all_chars(shellcode_string, '\n');	
	// Remove all '"'
	remove_all_chars(shellcode_string, '"');		
	
	// Parse to get number of shellcode bytes first
	int i = 0;
	*shellcode_size = 0;
	
	// Formatted shellcode ends when ';'
	while(shellcode_string[i] == '\\') {
		(*shellcode_size)++;
		i = i + 4;
	}	
	
	// Allocate memory for shellcode
	unsigned char *shellcode = (unsigned char *) malloc(*shellcode_size);	
		
	// Parse shellcode string again, convert hex representation into unsigned chars and copy into shellcode buffer		
	int j = 0;							// Shellcode write position
	char current_hex[5] = "0xaa";		
	char *endptr;						// Required by strtuol function
	
	// Shellcode string is of format \xaa\xbb\xcc...
	// Each loop iteration handles a 4-character chunk
	for(int i = 0; i < *shellcode_size; i++) {
		// Skip "\x" and interpret hex representation as unsigned char value 
		memcpy(&(current_hex[2]), &(shellcode_string[(4 * i) + 2]), 2);		
		shellcode[j] = (unsigned char) strtoul(current_hex, &endptr, 16);
		j++;
	}
	
	// Free file_content string
	free(file_content);	
	
	return shellcode;	
}


// Appends the given key to the specified file in a format that AVET expects as input.
// The output consists of an array unsigned char key[], which contains the key.
// The output file can be directly included in AVET sources to get key access.
// key_length is counted in bytes.
void append_key_to_file(const unsigned char *key, const int key_length, const char *filename) {
	// Append key to file
	FILE *shellcode_file = fopen(filename, "a");
		
	fputs("\n", shellcode_file);
	fputs("unsigned char key[] = \"", shellcode_file);
	
	char current_hex[4];
	
	for(int i = 0; i < key_length; i++) {
		sprintf(current_hex, "\\x%02x", key[i]);
		fputs(current_hex, shellcode_file);
	}
	
	fputs("\";", shellcode_file);
	fputs("\n", shellcode_file);
	fclose(shellcode_file);

}