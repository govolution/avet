#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../debug_print/debug_print.h"


// return pointer to payload
unsigned char* avet_decoder(const unsigned char *buffer, unsigned char *payload, int size)
{    
    DEBUG_PRINT("This is AVET decoder.\n");
	int j=0;
	payload=malloc((size/2));

	DEBUG_PRINT("decode_payload, size for malloc: %d\nPayload output:\n", size/2);
	
	int i=0;
	do
	{
		unsigned char temp[3]={0};
		sprintf((char*)temp,"%c%c",buffer[i],buffer[i+1]);
		payload[j] = strtoul(temp, NULL, 16);

		DEBUG_PRINT("%x",payload[j]);		

		i+=2;
		j++;
	} while(i<size);

	DEBUG_PRINT("\nAVET decoder finished.\n");

	return payload;
}


// Feature-system-compliant "wrapper" for legacy avet decoder
void decode_avet(const unsigned char *ciphertext, const int ciphertext_length, const unsigned char *key, const int key_length, unsigned char *plaintext) {
    unsigned char *decoded_payload = avet_decoder(ciphertext, plaintext, ciphertext_length);
    memcpy(plaintext, decoded_payload, ciphertext_length);
}
