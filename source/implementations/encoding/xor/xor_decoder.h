#pragma once

#include "../../debug_print/debug_print.h"


// XORs the ciphertext with the key and stores the result in plaintext.
// The key bytes are applied succesively. The ciphertext_length is not required to be a multiple of the key_length.
void decode_xor(const unsigned char *ciphertext, const int ciphertext_length, const unsigned char *key, const int key_length, unsigned char *plaintext) {
    DEBUG_PRINT("This is XOR decoder.\n");
	for(int i = 0; i < ciphertext_length; i++) {
		plaintext[i] = ciphertext[i] ^ key[i % key_length];
	}	
}
