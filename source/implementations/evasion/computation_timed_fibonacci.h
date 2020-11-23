#pragma once

#include <stdio.h>
#include <time.h>
#include "../debug_print/debug_print.h"


// Computes iterations of the Fibonacci series. Stops computations after approximately arg1 seconds.
// (Will likely produce false results for greater n due to modulo reduction inside integer boundaries)
//
// arg1:        Number of seconds to run the computation
void computation_timed_fibonacci(char *arg1) {
    int time = atoi(arg1);
    DEBUG_PRINT("Computing elements of the Fibonacci series for %s seconds...\n", arg1);

    unsigned int a = 0;
    unsigned int b = 1;
    unsigned int c = 0;

    clock_t start = clock();
    clock_t end;
    float elapsed_seconds = 0.0;

    unsigned int i = 2;
    while(1) {
        // Check elapsed time every 100 iterations
        if (i%100 == 0) {
            end = clock();
            elapsed_seconds = (float)(end - start) / CLOCKS_PER_SEC;
            if(elapsed_seconds > ((float) time)) {
                break;
            }
        }

        c = a + b;
        a = b;
        b = c;
        i++;
    }

    // Ensure that the computed value is required at the end of the data flow.
    printf("Final number is %u.", c);
}
