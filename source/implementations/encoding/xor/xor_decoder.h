#pragma once

// XORs the ciphertext with a single-byte key and stores the result in plaintext
void decode_xor(const unsigned char *ciphertext, const int ciphertext_length, const unsigned char *key, const int key_length, unsigned char *plaintext) {
	for(int i = 0; i < ciphertext_length; i++) {
		plaintext[i] = ciphertext[i] ^ key[0];
	}	
}
