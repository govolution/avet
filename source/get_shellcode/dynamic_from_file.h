#include <stdio.h>


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
	
	#ifdef PRINT_DEBUG
		printf("get_filesize, filesize %s: %d\n", fvalue, size);
	#endif

	return size;
}


// Returns pointer to buffer that contains the file content
// Automatically allocates memory for this
unsigned char *load_textfile(char *fvalue, int size)
{
	#ifdef PRINT_DEBUG
		printf("load_textfile called: fvalue: %s, size: %d\n", fvalue, size);
	#endif

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

	#ifdef PRINT_DEBUG
		printf("%s\n",buffer);
	#endif

	fclose(fp);	
	return buffer;
}


// Dynamic retrieval of raw shellcode data from file
// Dynamic in this context means that the shellcode to be executed is not statically compiled into the executable,
// but instead is retrieved from a file at run time.
//
// arg1 specifies the filename to fetch the shellcode from.
// shellcode_size receives the size of the shellcode in bytes.
unsigned char *get_shellcode(char *arg1, int *shellcode_size) {	
	// First command line argument specifies the file name to read from
	char *filename = arg1;	
	
	#ifdef PRINT_DEBUG
		printf("get shellcode from file %s\n", filename);
	#endif
	
	*shellcode_size = get_filesize(filename);
	// Memory allocation is handled by load_textfile
	return load_textfile(filename, *shellcode_size);		
}
