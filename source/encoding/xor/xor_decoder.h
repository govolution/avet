#pragma once

// XORs the ciphertext with a single-byte key and stores the result in plaintext
void decode_shellcode(const unsigned char *ciphertext, unsigned char *plaintext, const int ciphertext_length, const unsigned char key) {
	for(int i = 0; i < ciphertext_length; i++) {
		plaintext[i] = ciphertext[i] ^ key;
	}	
}
