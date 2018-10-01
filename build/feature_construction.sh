FEATURE_LOCATION="../evasion"


# Appends the content of a file to techniques.h
#
# First argument:	Name of the file (without file type suffix)
#
# Example usage:
# append_code fopen0_sandbox_evasion
# Appends the contents of the file evasion/fopen_sandbox_evasion.h to techniques.h
#
# BEWARE: Currenty only works for evasion techniques
function append_code() {
	cat evasion/$1.h >> techniques.h 	
}


# Writes a define into techniques.h
#
# First argument: 	Name of the define
# Second argument:  Value of the define
#
# Example usage:
# append_value MYDEFINE abcdef
#
# Writes (appends) #define MYDEFINE "abcdef" into techniques.h
#
# BEWARE: Currently only sets strings
function append_value() {
	echo "#define $1 \"$2\"" >> techniques.h
}


# Adds a feature (evasion technique etc.) to be included in the final avet executable
#
# First Argument: Name of the technique (= name of the file containing the respective code, without the file suffix)
function add_feature() {
	# Assume that techiques.h already exists and is properly set up
	
	# Copy contents of specified feature into techniques.h
	append_code $1	
}


# Resets the contents of the techniques.h file. To be called after payload compilation.
function cleanup_techniques() {
	echo "" > techniques.h
}
