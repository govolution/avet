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

# Install mingw crosscompiler
sudo apt install mingw-w64 -y
sudo apt install jq -y

echo "+++ Downloading and installing tdm-gcc via wine"
wget https://github.com/jmeubank/tdm-gcc/releases/download/v9.2.0-tdm64-1/tdm64-gcc-9.2.0.exe
wine tdm64-gcc-9.2.0.exe
rm tdm64-gcc-9.2.0.exe


while true; do
    read -p "Do you want to download the dependencies AVET needs? [y/n]" yn
    case $yn in
        [Yy]* ) # Install dependencies
            echo "+++ Downloading Mimikatz"
            curl -s https://api.github.com/repos/gentilkiwi/mimikatz/releases/latest \
            | jq -r '.assets[].browser_download_url' \
            | grep .'zip' \
            | wget -i -
            unzip -j mimikatz_trunk.zip x64/mimikatz.exe -d input/
            rm mimikatz_trunk.zip

            echo "+++ Downloading pe2shc"
            mkdir ../pe_to_shellcode
            curl -s https://api.github.com/repos/hasherezade/pe_to_shellcode/releases/latest \
            | jq -r '.assets[].browser_download_url' \
            | grep 'pe2shc' \
            | wget -i -
            mv pe2shc.exe ../pe_to_shellcode

            echo "+++ Cloning DKMC"
            git clone https://github.com/Mr-Un1k0d3r/DKMC.git
            mv DKMC/ ../ ; break;;
        [Nn]* )
            echo "Dependencies not installed!"
            break;;
        * ) echo "Please answer yes or no.";;
    esac
done

echo "+++ Finished setup procedure!"
