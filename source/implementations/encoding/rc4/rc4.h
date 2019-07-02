#pragma once

const int N = 256;   // 2^8


void swap(unsigned char *a, unsigned char *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}


int KSA(const unsigned char *key, unsigned char *S, const int key_length) {
    int j = 0;

    for(int i = 0; i < N; i++)
        S[i] = i;

    for(int i = 0; i < N; i++) {
        j = (j + S[i] + key[i % key_length]) % N;

        swap(&S[i], &S[j]);
    }

    return 0;
}


int PRGA(unsigned char *S, const unsigned char *plaintext, unsigned char *ciphertext, const int plaintext_length) {
    int i = 0;
    int j = 0;

    for(int n = 0; n < plaintext_length; n++) {
        i = (i + 1) % N;
        j = (j + S[i]) % N;

        swap(&S[i], &S[j]);
        int rnd = S[(S[i] + S[j]) % N];

        ciphertext[n] = rnd ^ plaintext[n];

    }

    return 0;
}


// RC4 encryption routine.
// Encrypts the given plaintext with the supplied key, and stores the result in ciphertext.
// Supports variable key length, within reason.
void RC4(const unsigned char *plaintext, const int plaintext_length, const unsigned char *key, const int key_length, unsigned char *ciphertext) {
    unsigned char S[N];
    KSA(key, S, key_length);
    PRGA(S, plaintext, ciphertext, plaintext_length);
}
