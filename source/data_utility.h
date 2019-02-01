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


// Writes the given data in array format into the specified file.
// ------
// THE PREVIOUS CONTENTS OF THE FILE WILL BE OVERWRITTEN!
// ------
// The output consists of an array unsigned char name[], where name can be specified by the corresponding parameter.
// The output file can be directly included in AVET sources to get data access.
// data_size is counted in bytes.
void data_to_file(const unsigned char *data, const int data_size, const char *filename, const char* array_name) {
	// Remove previous file contents
	FILE *data_file = fopen(filename, "w");
	fclose(data_file);
	
	// Write data to file
	data_file = fopen(filename, "a");
		
    // Assemble array name
    char array_descriptor[100];
    sprintf(array_descriptor, "unsigned char %s[] = \"", array_name);    

	fputs("\n", data_file);
	fputs(array_descriptor, data_file);
	
	char current_hex[4];
	
	for(int i = 0; i < data_size; i++) {
		sprintf(current_hex, "\\x%02x", data[i]);
		fputs(current_hex, data_file);
	}
	
	fputs("\";", data_file);
	fputs("\n", data_file);
	fclose(data_file);
}


// Writes the given data into the specified file.
// ------
// THE PREVIOUS CONTENS OF THE FILE WILL BE OVERWRITTEN!
// ------
// This will copy the data bytes "as is".
void data_to_file_raw(const unsigned char *data, const int data_size, const char *filename) {
	// Remove previous file contents
	FILE *data_file = fopen(filename, "w");
	fclose(data_file);
	
	// Write data to file
	data_file = fopen(filename, "a");	
	fwrite(data, 1, data_size, data_file);	
	
	fclose(data_file);
}


// Reads data from a file, given in format unsigned char buf[] = "\xaa\xbb\xcc...";
// The search pattern identifies the first backslash occurence in the file (hex representation of first byte) and reads the data from there.
// This implies that data will always be read from the first array of the specified format, regardless of array name.
// Returns pointer to the data read into memory. Required memory will be allocated by the function.
// data_size will be filled with the size of the data in bytes.
unsigned char *data_from_file(const char *filename, int *data_size) {		
	// Assume that file only contains array unsigned char buf[] = "\xaa\xbb\xcc...";
	FILE *data_file = fopen(filename, "r");   

	// Get file size
	fseek(data_file, 0L, SEEK_END);
	int file_size = ftell(data_file);
	rewind(data_file);	   
	
	// Allocate memory for string
	char *file_content = (char *) malloc(file_size + 1);
				
	// Get whole file content as string
	fread(file_content, 1, file_size, data_file);
	file_content[file_size] = '\0';
	
	// Close file
	fclose(data_file);	
	
	// Search for first occurence of backslash character (marks beginning of data string)
	char *data_string = strchr(file_content, '\\');	  
	
	// Format data string to make parsing easier:
	// Search for array end ';' and terminate string after that character
	strchr(data_string, ';')[1] = '\0';	
	// Remove all linebreaks
	remove_all_chars(data_string, '\n');	
	// Remove all '"'
	remove_all_chars(data_string, '"');		 

	// Parse to get number of data bytes first
	int i = 0;
	*data_size = 0;
	
	// Formatted data ends when ';'
	while(data_string[i] == '\\') {
		(*data_size)++;
		i = i + 4;
	}	
	
	// Allocate memory for data
	unsigned char *data = (unsigned char *) malloc(*data_size);	
		
	// Parse data string again, convert hex representation into unsigned chars and copy into data buffer		
	int j = 0;							// data write position
	char current_hex[5] = "0xaa";		
	char *endptr;						// Required by strtuol function
	
	// data string is of format \xaa\xbb\xcc...
	// Each loop iteration handles a 4-character chunk
	for(int i = 0; i < *data_size; i++) {
		// Skip "\x" and interpret hex representation as unsigned char value 
		memcpy(&(current_hex[2]), &(data_string[(4 * i) + 2]), 2);		
		data[j] = (unsigned char) strtoul(current_hex, &endptr, 16);
		j++;
	}
	
	// Free file_content string
	free(file_content);	
	
	return data;	
}


// Reads raw data data from a file.
// Returns pointer to the data in memory. Required memory will be allocated by the function.
// data_size will be filled with the size of the data in bytes.
unsigned char *data_from_file_raw(const char *filename, int *data_size) {
	// Assume that file only contains the data data
	FILE *data_file = fopen(filename, "r");
	
	// Get file size = data size
	fseek(data_file, 0L, SEEK_END);
	*data_size = ftell(data_file);
	rewind(data_file);
	
	// Allocate memory for data
	unsigned char *data = (unsigned char *) malloc(*data_size);
	
	// Read data from file
	fread(data, 1, *data_size, data_file);
	
	fclose(data_file);
	return data;
}

