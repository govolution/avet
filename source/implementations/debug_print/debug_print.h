#pragma once


// Defines the DEBUG_PRINT macro.
// DEBUG_PRINT writes to stdout by default, or into a logfile instead if the DEBUG_TO_FILE define is set.
// Use DEBUG_PRINT like printf:     DEBUG_PRINT("My string: %s\n", my_string);


#ifdef DEBUG
    #include <stdio.h>        
    
    #ifdef DEBUG_TO_FILE
        #include <stdarg.h>

        // Definition of LOGFILE should be enforced by enable_debug_output.
        // Check anyways in case something went wrong.
        #ifndef LOGFILE
            #define LOGFILE "C:\\users\\public\\avetdbg.txt"
        #endif

        // Debug output is written to file instead when DEBUG_TO_FILE is set (as in avetsvc)       
        int DEBUG_PRINT(char *format, ...) {
            va_list args;
            va_start(args, format);   
         
            FILE *logfile;            
		    logfile = fopen(LOGFILE, "a+");
		    if(logfile == NULL) {
			    return -1;
		    }
		    vfprintf(logfile, format, args);
		    fclose(logfile);

            va_end(args);

		    return 0;
	    }        
    #else
        // Debug output via printf
        #define DEBUG_PRINT(...) printf(__VA_ARGS__)        
    #endif
#else
    // Substitute function call with 0 expression, so that debug strings are purged from the executable   
    #define DEBUG_PRINT(...) 0
#endif
