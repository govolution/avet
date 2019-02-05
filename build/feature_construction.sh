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
NUM_TECHNIQUES=0
#
# First Argument: 	Name of the technique (= name of the file containing the respective code, without the file suffix)
function add_evasion() {
	# Set include in evasion.include to import the selected evasion technique implementation
	printf "\n#include \"../implementations/evasion/$1.h\"\n" >> source/evasion/evasion.include
	
	# Write an assignment of the selected function to the evasion function array into the evasion.assign file
	printf "\nevasion_functions[$NUM_TECHNIQUES] = $1;\n" >> source/evasion/evasion.assign
	
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
		
	# Set include in get_key.include to import the needed data retrieval method
	printf "\n#include \"../implementations/retrieve_data/$1.h\"\n" >> source/get_key/get_key.include
	# Write an assignment of the selected function to get_key into the get_key.assign file
	printf "\nget_key = $1;\n" >> source/get_key/get_key.assign
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
# First Argument: 	Name of the payload decoder (= name of the folder containinig the respective code)
function set_decoder() {
	# Set include in decode_payload.include to import the needed decoder method
	printf "\n#include \"../implementations/encoding/$1/$1_decoder.h\"\n" >> source/decode_payload/decode_payload.include
	# Write an assignment of the selected function to decode_payload into the decode_payload.assign
	printf "\ndecode_payload = decode_$1;\n" >> source/decode_payload/decode_payload.assign	
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


# Resets the contents of the administrativa include and assign files. To be called after payload compilation.
function cleanup_techniques() {
	echo "" > source/evasion/evasion.include
	echo "" > source/evasion/evasion.assign	
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
