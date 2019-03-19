#!/bin/bash
# This is a setup script to get AVET ready to use on a 64-bit Kali system.
# You still have to "next" through the tdm-gcc installer GUI, though.

echo "+++ Preparing AVET for use..."

echo "+++ Installing wine and wine32"
# add missing sources for wine32
echo "deb http://http.kali.org/kali kali-rolling main contrib non-free" >> /etc/apt/sources.list
echo "deb http://old.kali.org/kali sana main non-free contrib" >> /etc/apt/sources.list
echo "deb http://old.kali.org/kali moto main non-free contrib" >> /etc/apt/sources.list
dpkg --add-architecture i386
apt update
apt install wine -y
apt install wine32 -y

echo "+++ Downloading and installing tdm-gcc via wine"
wget https://downloads.sourceforge.net/project/tdm-gcc/TDM-GCC%20Installer/tdm64-gcc-5.1.0-2.exe
wine tdm64-gcc-5.1.0-2.exe
rm tdm64-gcc-5.1.0-2.exe
echo "+++ Finished setup procedure!"
