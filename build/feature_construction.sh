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


# Adds an evasion technique to be included in the final avet executable
#
# NUM_TECHNIQUES is a counter that keeps track on how many (not necessarily different individual) techniques were added in total.
NUM_TECHNIQUES=0
#
# First Argument: 	Name of the technique (= name of the file containing the respective code, without the file suffix)
function add_evasion() {
	# Set include in evasion.include to import the selected evasion technique implementation
	printf "\n#include \"implementations/evasion/$1.h\"\n" >> source/evasion/evasion.include
	
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
	
	# If retrieval from file, assume that file sc.txt contains shellcode that is already in a c-compatible format
	if [ $1 = "static_from_file" ]
	then
    	cat sc.txt >> source/get_shellcode/get_shellcode.include		
	fi	
		
	# Set include in get_shellcode.include to import the needed data retrieval method
	printf "\n#include \"implementations/retrieve_data/$1.h\"\n" >> source/get_shellcode/get_shellcode.include
	# Write an assignment of the selected function to get_shellcode into the get_shellcode.assign file
	printf "\nget_shellcode = $1;\n" >> source/get_shellcode/get_shellcode.assign
}


# Specifies how the payload shellcode will be called
#
# First Argument: 	Name of the shellcode binding technique (= name of the file containinig the respective code, without the file suffix)
function shellcode_binding() {
	# Assume that shellcode_binding.h already exists and is properly set up
	
	# Copy contents of specified feature into shellcode_binding.h
	cat source/shellcode_binding/$1.h >> source/shellcode_binding.h
}


# Specifies which decoder function should be applied to the shellcode
#
# First Argument: 	Name of the shellcode decoder (= name of the folder containinig the respective code)
function set_decoder() {
	# Assume that encoding.h already exists and is properly set up
	
	# Copy the specified decoding function into encoding.h
	cat source/encoding/$1/$1_decoder.h >> source/encoding.h
}


# Encodes the shellcode in the specified file by applying the specified encoding method
#
# First Argument:	Name of the encoding technique to be applied (= name of the folder containing the respective code)
# Second Argument:	Name of the file containing the shellcode to be encoded
# Third Argument:	Name of the file where the encoded shellcode shall be written to
function encode_shellcode() {
	source/encoding/$1_encoder $2 $3
}


# Resets the contents of the techniques.h and shellcode_binding.h files. To be called after payload compilation.
function cleanup_techniques() {
	echo "" > source/evasion/evasion.include
	echo "" > source/evasion/evasion.assign	
	echo "" > source/get_shellcode/get_shellcode.include
	echo "" > source/get_shellcode/get_shellcode.assign	
}
