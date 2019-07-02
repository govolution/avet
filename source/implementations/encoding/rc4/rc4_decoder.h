#pragma once

#include "../../debug_print/debug_print.h"
#include "rc4.h"


// RC4-decrypts the ciphertext with the key and stores the result in plaintext.
// To do this, the RC4 function is applied reverse to encryption, with the ciphertext posing as the plain text, and vice versa.
void decode_rc4(const unsigned char *ciphertext, const int ciphertext_length, const unsigned char *key, const int key_length, unsigned char *plaintext) {
    DEBUG_PRINT("This is RC4 decoder.\n");
    // Function signature:
    // void RC4(const unsigned char *plaintext, const int plaintext_length, const unsigned char *key, const int key_length, unsigned char *ciphertext)
    RC4(ciphertext, ciphertext_length, key, key_length, plaintext);
}
