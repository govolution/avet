#pragma once

// Dummy function to handle options where no encoding/decoding is applied at all
void decode_shellcode(const unsigned char *ciphertext, unsigned char *plaintext, const int ciphertext_length, const unsigned char key) {}
