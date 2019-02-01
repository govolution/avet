#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "../../debug_print/debug_print.h"


// Searches for the last occurence of the '/' character and returns the string remaining after that last occurence
// Note that the return value will be pointing to the same string
char *get_filename_from_url(char *url) {
    int index = -1;
    int i = 0;

    while(url[i] != '\0')
    {
        if(url[i] == '/')
        {
            index = i;
        }
        i++;
    }

    return &url[index + 1];
}


// Returns the content size of the file given by fvalue
int get_filesize(char *fvalue) {
	int size,rc1;
	FILE *fp1 = fopen(fvalue, "rb");
	if (fp1 == NULL)
	{
		DEBUG_PRINT("get_filesize, %s not found\n", fvalue);
		return 0;
	}
	for (size = 0; (rc1 = getc(fp1)) != EOF; size++) {}
	fclose(fp1);
	
	DEBUG_PRINT("get_filesize, filesize %s: %d\n", fvalue, size);

	return size;
}


// Returns pointer to buffer that contains the file content
// Automatically allocates memory for this
unsigned char *load_textfile(char *fvalue, int size) {
	DEBUG_PRINT("load_textfile called: fvalue: %s, size: %d\n", fvalue, size);
	
	//allocate buffer, open file, read file to the buffer, close the file
	unsigned char *buffer = (unsigned char*) malloc(size+1);
	int i, rc;

	for (i=0; i<size; i++)
		buffer[i]=0x0;

	FILE *fp = fopen(fvalue, "rb");
	if (fp == NULL)
	{
		DEBUG_PRINT("load_textfile, %s not found\n", fvalue);
		return 0;
	}

	for (i=0; i<size; i++)
	{
		rc = getc(fp);
		buffer[i] = rc;
	}
	
	DEBUG_PRINT("%s\n",buffer);	

	fclose(fp);	
	return buffer;
}
