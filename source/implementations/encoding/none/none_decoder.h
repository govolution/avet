#pragma once

#include <string.h>
#include "../../debug_print/debug_print.h"


// Dummy function to handle options where no encoding/decoding is applied at all
void decode_none(const unsigned char *ciphertext, const int ciphertext_length, const unsigned char *key, const int key_length, unsigned char *plaintext) {
    DEBUG_PRINT("decode_none function called \n");
    memcpy(plaintext, ciphertext, ciphertext_length);
}
