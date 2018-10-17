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


# Adds a feature (evasion technique etc.) to be included in the final avet executable
#
# First Argument: 	Name of the technique (= name of the file containing the respective code, without the file suffix)
function add_feature() {
	# Assume that techiques.h already exists and is properly set up
	
	# Copy contents of specified feature into techniques.h
	cat source/evasion/$1.h >> source/techniques.h	
}


# Specifies where to get the shellcode from
#
# First Argument: 	Name of the technique (= name of the file containing the respective code, without the file suffix)
# Second Argument:  Can be used to deliver further data about the source, e.g. the file name when retrieving from a file or the URL when downloading from a URL
function shellcode_source() {
	printf "\n\n" >> source/get_shellcode.h	
	
	# If retrieval from file, assume that file sc.txt contains shellcode that is already in a c-compatible format
	if [ $1 = "static_from_file" ]
	then
    	cat sc.txt >> source/get_shellcode.h		
	fi	
		
	# Copy code of retrieval method
	cat source/get_shellcode/$1.h >> source/get_shellcode.h
}


# Specifies how the payload shellcode will be called
#
# First Argument: 	Name of the shellcode binding technique (= name of the file containinig the respective code, without the file suffix)
function shellcode_binding() {
	# Assume that shellcode_binding.h already exists and is properly set up
	
	# Copy contents of specified feature into shellcode_binding.h
	cat source/shellcode_binding/$1.h >> source/shellcode_binding.h
}


# Resets the contents of the techniques.h and shellcode_binding.h files. To be called after payload compilation.
function cleanup_techniques() {
	echo "" > source/techniques.h
	echo "" > source/shellcode_binding.h
	echo "" > source/get_shellcode.h
}
