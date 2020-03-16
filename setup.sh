#!/bin/bash
# This is a setup script to get AVET ready to use on a 64-bit Kali system.
# You still have to "next" through the tdm-gcc installer GUI, though.

echo "+++ Preparing AVET for use..."

echo "+++ Installing wine and wine32"
# add missing sources for wine32
sudo dpkg --add-architecture i386
sudo apt update
sudo apt install wine -y
sudo apt install wine32 -y

echo "+++ Downloading and installing tdm-gcc via wine"
wget https://github.com/jmeubank/tdm-gcc/releases/download/v9.2.0-tdm64-1/tdm64-gcc-9.2.0.exe
wine tdm64-gcc-9.2.0.exe
rm tdm64-gcc-9.2.0.exe
echo "+++ Finished setup procedure!"
