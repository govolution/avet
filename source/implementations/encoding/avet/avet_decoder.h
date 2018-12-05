#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../debug_print/debug_print.h"


// return pointer to shellcode
unsigned char* avet_decoder(const unsigned char *buffer, unsigned char *shellcode, int size)
{    
    DEBUG_PRINT(("This is AVET decoder.\n"));
	int j=0;
	shellcode=malloc((size/2));

	DEBUG_PRINT(("decode_shellcode, size for malloc: %d\nShellcode output:\n", size/2));
	
	int i=0;
	do
	{
		unsigned char temp[3]={0};
		sprintf((char*)temp,"%c%c",buffer[i],buffer[i+1]);
		shellcode[j] = strtoul(temp, NULL, 16);

		DEBUG_PRINT(("%x",shellcode[j]));		

		i+=2;
		j++;
	} while(i<size);

	DEBUG_PRINT(("AVET decoder finished.\n"));

	return shellcode;
}


// Feature-system-compliant "wrapper" for legacy avet decoder
void decode_avet(const unsigned char *ciphertext, const int ciphertext_length, const unsigned char *key, const int key_length, unsigned char *plaintext) {
    unsigned char *decoded_shellcode = avet_decoder(ciphertext, plaintext, ciphertext_length);
    memcpy(plaintext, decoded_shellcode, ciphertext_length);
}
