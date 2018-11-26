#pragma once


// Use with double parentheses like
// DEBUG_PRINT(("output 1: %s, output 2: %d\n", "hello", 1337));

#ifdef DEBUG
# include <stdio.h>
# define DEBUG_PRINT(x) printf x
#else
# define DEBUG_PRINT(x) do {} while (0)
#endif
