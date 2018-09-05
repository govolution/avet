#!/bin/bash
# This is a setup script to get AVET ready to use on a 64-bit Kali system.
# You still have to "next" through the tdm-gcc installer GUI, though.

echo "+++ Preparing AVET for use..."

echo "+++ Building make_avet executable"
gcc -o make_avet make_avet.c

echo "+++ Building make_avetsvc executable"
gcc -o make_avetsvc make_avetsvc.c

echo "+++ Installing wine and wine32"
dpkg --add-architecture i386
apt update
apt install wine -y
apt install wine32 -y

echo "+++ Downloading and installing tdm-gcc via wine"
wget https://downloads.sourceforge.net/project/tdm-gcc/TDM-GCC%20Installer/tdm64-gcc-5.1.0-2.exe
wine tdm64-gcc-5.1.0-2.exe
rm tdm64-gcc-5.1.0-2.exe
echo "+++ Finished setup procedure!"