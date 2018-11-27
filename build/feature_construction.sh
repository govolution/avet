# Appends a define at the end of a the specified file
#
# First argument: 	Name of the define
# Second argument:  Value of the define
# Third argument:	Name of the file to append to
#
# Example usage:
# append_value MYDEFINE abcdef techniques.h
#
# Writes (appends) #define MYDEFINE "abcdef" into techniques.h
#
# BEWARE: Currently only sets strings
function append_value() {
	printf "\n#define $1 \"$2\"\n" >> $3
}


# Enables the PRINT_DEBUG macro in the respective include file.
# Setting this, the generated executable will generate verbose stdoutput.
function enable_debug_print() {
	printf "\n#define DEBUG \n" >> source/debug_print/debug_print.include
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


# Specifies where to get the shellcode from
#
# First Argument: 	Name of the technique (= name of the file containing the respective code, without the file suffix)
# Second Argument:  Can be used to deliver further data about the source, e.g. the file name when retrieving from a file or the URL when downloading from a URL
function set_shellcode_source() {
	printf "\n\n" >> source/get_shellcode/get_shellcode.include	
	
	# If retrieval from file, assume that file sc.txt contains shellcode that is already in C-compatible format
	if [ $1 = "static_from_file" ]
	then
    	cat $2 >> source/get_shellcode/get_shellcode.include		
	fi	
		
	# Set include in get_shellcode.include to import the needed data retrieval method
	printf "\n#include \"../implementations/retrieve_data/$1.h\"\n" >> source/get_shellcode/get_shellcode.include
	# Write an assignment of the selected function to get_shellcode into the get_shellcode.assign file
	printf "\nget_shellcode = $1;\n" >> source/get_shellcode/get_shellcode.assign
}

# Specifies where to get the crypto key from
#
# First Argument:	Name of the technique (=name of the file containing the respective code, without the file suffix)
# Second Argument:	Can be used to deliver further data about the source, e.g. the file name when retrieving from a file or the URL when downloading from a URL
function set_key_source() {
	printf "\n\n" >> source/get_key/get_key.include	
	
	# If retrieval from file, assume that file key.txt contains key that is already in C-compatible format
	if [ $1 = "static_from_file" ]
	then
    	cat $2 >> source/get_key/get_key.include		
	fi	
		
	# Set include in get_key.include to import the needed data retrieval method
	printf "\n#include \"../implementations/retrieve_data/$1.h\"\n" >> source/get_key/get_key.include
	# Write an assignment of the selected function to get_shellcode into the get_shellcode.assign file
	printf "\nget_key = $1;\n" >> source/get_key/get_key.assign
}


# Specifies how the payload shellcode will be called
#
# First Argument: 	Name of the shellcode binding technique (= name of the file containinig the respective code, without the file suffix)
function set_shellcode_binding() {
	# Set include in shellcode_binder.include to import the implemented binding function
	printf "\n#include \"../implementations/shellcode_binder/$1.h\"\n" >> source/shellcode_binder/shellcode_binder.include
	# Write an assignment of the selected function to shellcode_binder into the shelcode_binder.assign file
	printf "\nshellcode_binder = $1;\n" >> source/shellcode_binder/shellcode_binder.assign
}


# Specifies which decoder function should be applied to the shellcode
#
# First Argument: 	Name of the shellcode decoder (= name of the folder containinig the respective code)
function set_decoder() {
	# Set include in decode_shellcode.include to import the needed decoder method
	printf "\n#include \"../implementations/encoding/$1/$1_decoder.h\"\n" >> source/decode_shellcode/decode_shellcode.include
	# Write an assignment of the selected function to decode_shellcode into the decode_shellcode.assign
	printf "\ndecode_shellcode = decode_$1;\n" >> source/decode_shellcode/decode_shellcode.assign	
}


# Encodes the shellcode in the specified file by applying the specified encoding method
#
# First Argument:	Name of the encoding technique to be applied (= name of the folder containing the respective code)
# Second Argument:	Name of the file containing the shellcode to be encoded
# Third Argument:	Name of the file where the encoded shellcode shall be written to
# Fourth Argument:	Name of the file where the generated key shall be written to
function encode_shellcode() {
	source/implementations/encoding/$1/$1_encoder $2 $3 $4
}


# Resets the contents of the techniques.h and shellcode_binding.h files. To be called after payload compilation.
function cleanup_techniques() {
	echo "" > source/evasion/evasion.include
	echo "" > source/evasion/evasion.assign	
	echo "" > source/get_shellcode/get_shellcode.include
	echo "" > source/get_shellcode/get_shellcode.assign	
	echo "" > source/get_key/get_key.include
	echo "" > source/get_key/get_key.assign
	echo "" > source/decode_shellcode/decode_shellcode.include
	echo "" > source/decode_shellcode/decode_shellcode.assign
	echo "" > source/shellcode_binder/shellcode_binder.include
	echo "" > source/shellcode_binder/shellcode_binder.assign
	echo "" > source/debug_print/debug_print.include
}
