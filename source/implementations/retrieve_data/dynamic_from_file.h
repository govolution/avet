#pragma once

#include <stdio.h>
#include "../debug_print/debug_print.h"


int get_filesize(char *fvalue)
{
	int size,rc1;
	FILE *fp1 = fopen(fvalue, "rb");
	if (fp1 == NULL)
	{
		printf("get_filesize, %s not found\n", fvalue);
		return 0;
	}
	for (size = 0; (rc1 = getc(fp1)) != EOF; size++) {}
	fclose(fp1);
	
	DEBUG_PRINT(("get_filesize, filesize %s: %d\n", fvalue, size));
	
	return size;
}


// Returns pointer to buffer that contains the file content
// Automatically allocates memory for this
unsigned char *load_textfile(char *fvalue, int size)
{
	DEBUG_PRINT(("load_textfile called: fvalue: %s, size: %d\n", fvalue, size));
	
	//allocate buffer, open file, read file to the buffer, close the file
	unsigned char *buffer = (unsigned char*) malloc(size+1);
	int i, rc;

	for (i=0; i<size; i++)
		buffer[i]=0x0;

	FILE *fp = fopen(fvalue, "rb");
	if (fp == NULL)
	{
		printf("load_textfile, %s not found\n", fvalue);
		return 0;
	}

	for (i=0; i<size; i++)
	{
		rc = getc(fp);
		buffer[i] = rc;
	}

	DEBUG_PRINT(("%s\n",buffer));

	fclose(fp);	
	return buffer;
}


// Dynamic retrieval of data from file
// Dynamic in this context means that the data is not statically compiled into the executable,
// but instead is retrieved from a file at run time.
//
// arg1 specifies the filename to fetch the data from.
// data_size receives the size of the data in bytes.
unsigned char *dynamic_from_file(char *arg1, int *data_size) {	
	// First command line argument specifies the file name to read from
	char *filename = arg1;	
	
	DEBUG_PRINT(("Dynamically retrieving data from file %s...\n", filename));
	
	*data_size = get_filesize(filename);
	// Memory allocation is handled by load_textfile
	return load_textfile(filename, *data_size);		
}
