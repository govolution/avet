#pragma once

// Dummy function to handle options where no encoding/decoding is applied at all
void decode_none(const unsigned char *ciphertext, const int ciphertext_length, const unsigned char *key, const int key_length, unsigned char *plaintext) {}
