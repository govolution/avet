#pragma once

#include <stdio.h>
#include "../debug_print/debug_print.h"


// Compute arg1 iterations of the Fibonacci series.
// (Will likely produce false results for greater n due to modulo reduction inside integer boundaries)
//
// arg1:        Number of iterations to compute
void computation_fibonacci(char *arg1) {
    int n = atoi(arg1);
    DEBUG_PRINT("Computing the %sth number of the fibonacci series...\n", arg1);

    unsigned int a = 0;
    unsigned int b = 1;
    unsigned int c = 0;

    int i = 2;
    while (i < n) {
        c = a + b;
        a = b;
        b = c;
        i++;
    }

    // Ensure that the computed value is used at the end of the data flow to avoid dead code.
    printf("Number is %u.", c);
}
