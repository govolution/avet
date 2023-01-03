# Enables the PRINT_DEBUG macro in the respective include file.
# Setting this, the generated executable will generate verbose stdoutput.
# Using the arguments, debug output is written into a file instead, which can be specified.
#
# First Argument (optional):	Set as to_file to write debug output into the file specified in argument 2 instead.
# Second Argument (optional):	Specifies the file to write debug output into.
#					Use forward dashes:	enable_debug_print to_file C:/path/file.txt
function enable_debug_print() {
	printf "\n#define DEBUG \n" >> source/debug_print/debug_print.include
	
	if [ "$1" = "to_file" ]
	then
		printf "\n#define DEBUG_TO_FILE \n" >> source/debug_print/debug_print.include
		printf "\n#define LOGFILE \"$2\"" >> source/debug_print/debug_print.include
	fi	
}

# Adds an evasion technique to be included in the final avet executable
#
# NUM_TECHNIQUES is a counter that keeps track on how many (not necessarily different individual) techniques were added in total.
# This is needed to fill the evasion function (argument) arrays in the evasion.assign file.
NUM_TECHNIQUES=0
#
# First Argument: 	Name of the technique (= name of the file containing the respective code, without the file suffix)
# Second Argument:  Specify a value that will be handed to the evasion function.
#                   The value will be handed over as a C string!
#                   Put your argument in single quotes, otherwise specify like a C string.
#                   As an example, you can specify the hostname to use in the gethostbyname evasion technique here.
function add_evasion() {
 	# Set include in evasion.include to import the selected evasion technique implementation
	printf "\n#include \"../implementations/evasion/$1.h\"\n" >> source/evasion/evasion.include
	
	# Write an assignment of the selected function to the evasion function array into the evasion.assign file
	printf "\nevasion_functions[$NUM_TECHNIQUES] = $1;\n" >> source/evasion/evasion.assign

    # Write an assignment of the (optional) function argument to the argument array into the evasion.assign file.
    # If $2 is not specified, en empty string will be written to the argument array.
    # The assignment is not done statically, but via strcpy. This keeps individual assignment operations independent from each other.
    printf "\n#include <string.h>\n" >> source/evasion/evasion.assign
    printf "\nstrcpy(evasion_function_args[$NUM_TECHNIQUES], \"%s\");\n" $2 >> source/evasion/evasion.assign
	
	# Increment technique counter
	((NUM_TECHNIQUES=NUM_TECHNIQUES+1))
}

# Resets the evasion technique counter.
# This is mandatory before each individual compilation when compiling multiple executables in one build script!
function reset_evasion_technique_counter() {
	NUM_TECHNIQUES=0
}

# Specifies where to get the payload from
#
# First Argument: 	Name of the technique (= name of the file containing the respective code, without the file suffix)
# Second Argument:  Can be used to deliver further data about the source, e.g. the file name when retrieving from a file or the URL when downloading from a URL
function set_payload_source() {
	printf "\n\n" >> source/get_payload/get_payload.include	
	
	# If payload is included statically, write payload data into get_payload.include file and set respective define
	if [ $1 = "static_from_file" ]
	then
		cat $2 >> source/static_data/static_data.include
		printf "\n#define STATIC_PAYLOAD \n" >> source/static_data/static_data.include
	fi	

    # Feature to set the payload directly in the build script.
    # We don't expect this to be used much, but wanted to align it with the outher set_source functions.
    # This avoids the file inclusion hustle.
    # The specified payload will be statically included in the executable.
    if [ $1 = "static_from_here" ]
	then
        printf "\n unsigned char buf[] = \"$2\";\n" >> source/static_data/static_data.include
		printf "\n#define STATIC_PAYLOAD \n" >> source/get_payload/get_payload.include
	fi
		
	# Set include in get_payload.include to import the needed data retrieval method
	printf "\n#include \"../implementations/retrieve_data/$1.h\"\n" >> source/get_payload/get_payload.include
	# Write an assignment of the selected function to get_payload into the get_payload.assign file
	printf "\nget_payload = $1;\n" >> source/get_payload/get_payload.assign
}

# Specifies where to get the crypto key from
#
# First Argument:	Name of the technique (=name of the file containing the respective code, without the file suffix)
# Second Argument:	Can be used to deliver further data about the source, e.g. the file name when retrieving from a file or the URL when downloading from a URL
function set_key_source() {
	printf "\n\n" >> source/get_key/get_key.include	
	
	# If key is included statically, write key data into get_key.include file and set respective define
	if [ $1 = "static_from_file" ]
	then
    	cat $2 >> source/static_data/static_data.include
		printf "\n#define STATIC_KEY \n" >> source/static_data/static_data.include
	fi	

    # Feature to set the key directly in the build script.
    # This avoids the file inclusion hustle.
    # The specified key will be statically included in the executable.
    if [ $1 = "static_from_here" ]
	then
        printf "\n unsigned char key[] = \"$2\";\n" >> source/static_data/static_data.include
		printf "\n#define STATIC_KEY \n" >> source/get_key/get_key.include
	fi
		
	# Set include in get_key.include to import the needed data retrieval method
	printf "\n#include \"../implementations/retrieve_data/$1.h\"\n" >> source/get_key/get_key.include
	# Write an assignment of the selected function to get_key into the get_key.assign file
	printf "\nget_key = $1;\n" >> source/get_key/get_key.assign
}


# Specifies where to get the command from
#
# First Argument:   Name of the technique (=name of the file containing the respective code, without the file suffix)
# Second Argument:  Can be used to deliver further data about the source, e.g. the file name when retrieving from a file or the URL when downloading from a URL
function set_command_source() {
    printf "\n\n" >> source/get_command/get_command.include

    # If command is included statically, write command into get_command.include file and set respective define
    if [ $1 = "static_from_file" ]
	then
    	cat $2 >> source/static_data/static_data.include
		printf "\n#define STATIC_COMMAND \n" >> source/get_command/get_command.include
	fi 

    # Feature to set the command directly in the build script.
    # This avoids the file inclusion hustle.
    # The specified command will be statically included in the executable.
    if [ $1 = "static_from_here" ]
	then
        printf "\n unsigned char command[] = \"$2\";\n" >> source/static_data/static_data.include
		printf "\n#define STATIC_COMMAND \n" >> source/get_command/get_command.include
	fi

    # Set include in get_command.include to import the needed data retrieval method
    printf "\n#include \"../implementations/retrieve_data/$1.h\"\n" >> source/get_command/get_command.include
    # Write an assignment of the selected function to get_command into the get_command.assign file
    printf "\nget_command = $1;\n" >> source/get_command/get_command.assign
}


# Specifies where to get the additional payload information from.
# An example for such additional information would be the PID of the target process to hollow into when performing process hollowing.
#
# First Argument: 	Name of the technique (=name of the file containing the respective code, without the file suffix)
# Second Argument:	Can be used to deliver further data about the source, e.g. the file name when retrieving from a file or the URL when downloading from a URL
function set_payload_info_source() {
	printf "\n\n" >> source/get_payload_info/get_payload_info.include
	
	# If payload info is included statically, write payload info data into get_payload_info.include file and set the respective define
	if [ $1 = "static_from_file" ]
	then
		cat $2 >> source/static_data/static_data.include
		printf "\n#define STATIC_PAYLOAD_INFO \n" >> source/static_data/static_data.include
	fi

    # Feature to set the payload info directly in the build script.
    # This avoids the file inclusion hustle.
    # The specified payload info will be statically included in the executable.
    if [ $1 = "static_from_here" ]
	then
        printf "\n unsigned char payload_info[] = \"$2\";\n" >> source/static_data/static_data.include
		printf "\n#define STATIC_PAYLOAD_INFO \n" >> source/get_payload_info/get_payload_info.include
	fi
	
	# Set include in get_payload_info.include to import the needed data retrieval method
	printf "\n#include \"../implementations/retrieve_data/$1.h\"\n" >> source/get_payload_info/get_payload_info.include
	# Write an assignment of the selected function to get_payload_info into the get_payload_info.assign file
	printf "\nget_payload_info = $1;\n" >> source/get_payload_info/get_payload_info.assign
}


# Writes to file either a randomy generated or manually preset key.
#
# First Argument:	(random|preset)
#		random:		Generates a random byte sequence. Sequence length in bytes is specified in the second argument.
#		preset:		Allows to enter a preset key, given in the second argument. Input format is a hex string, such as aabbccdd...
# Second Argument:	Either specified the key length in bytes, or a preset key as hex string, depending on first argument choice.
# Third Argument:	Name of the output file to write generated raw key data into.
function generate_key() {
	./tools/generate_key/generate_key $1 $2 $3
}


# Specifies how the payload will be executed
#
# First Argument: 	Name of the payload execution method (= name of the file containinig the respective code, without the file suffix)
function set_payload_execution_method() {
	# Set include in payload_execution_method.include to import the implemented binding function
	printf "\n#include \"../implementations/payload_execution_method/$1.h\"\n" >> source/payload_execution_method/payload_execution_method.include
	# Write an assignment of the selected function to payload_execution_method into the payload_execution_method.assign file
	printf "\npayload_execution_method = $1;\n" >> source/payload_execution_method/payload_execution_method.assign
}


# Specifies which decoder function should be applied to the payload
#
# First Argument: 	Name of the payload decoder (= name of the folder containing the respective code)
function set_decoder() {
	# Set include in decode_payload.include to import the needed decoder method
	printf "\n#include \"../implementations/encoding/$1/$1_decoder.h\"\n" >> source/decode_payload/decode_payload.include
	# Write an assignment of the selected function to decode_payload into the decode_payload.assign
	printf "\ndecode_payload = decode_$1;\n" >> source/decode_payload/decode_payload.assign	
}


# Specifies which command execution function should be used
#
# First Argument:   Name of the execution technique (= name of the folder containing the respective code) 
function set_command_exec() {
    # Set include in command_exec.include to import the needed command execution method
    printf "\n#include \"../implementations/command_exec/$1.h\"\n" >> source/command_exec/command_exec.include
    # Write an assignment of the selected function to command_exec into the command_exec.assign
    printf "\ncommand_exec = $1;\n" >> source/command_exec/command_exec.assign
}


# Encodes the payload in the specified file by applying the specified encoding method
#
# First Argument:	Name of the encoding technique to be applied (= name of the folder containing the respective code)
# Second Argument:	Name of the file containing the payload to be encoded
# Third Argument:	Name of the file where the encoded payload shall be written to
# Fourth Argument:	Name of the file containing the key to be applied
function encode_payload() {
	source/implementations/encoding/$1/$1_encoder $2 $3 $4
}

function gen_adversarial_exe() {
	if [[ $# -eq 0 ]] ; then
    	echo -e '\033[0;31mError at gen_adversarial_exe: No attack specified\033[0m'
	else	
		python3 source/implementations/gen_adversarial_exe/$1.py $2 $3
	fi
}

genetic_optimizer="python3 source/implementations/gen_adversarial_exe/genetic_optimizer.py"


# Resets the contents of the administrativa include and assign files. To be called after payload compilation.
function cleanup_techniques() {
	echo "" > source/evasion/evasion.include
	echo "" > source/evasion/evasion.assign	
    echo "" > source/command_exec/command_exec.include
    echo "" > source/command_exec/command_exec.assign  
    echo "" > source/get_command/get_command.include
    echo "" > source/get_command/get_command.assign 
	echo "" > source/get_payload/get_payload.include
	echo "" > source/get_payload/get_payload.assign	
	echo "" > source/get_key/get_key.include
	echo "" > source/get_key/get_key.assign
	echo "" > source/decode_payload/decode_payload.include
	echo "" > source/decode_payload/decode_payload.assign
	echo "" > source/payload_execution_method/payload_execution_method.include
	echo "" > source/payload_execution_method/payload_execution_method.assign
	echo "" > source/get_payload_info/get_payload_info.include
	echo "" > source/get_payload_info/get_payload_info.assign
	echo "" > source/debug_print/debug_print.include
	echo "" > source/static_data/static_data.include
}
