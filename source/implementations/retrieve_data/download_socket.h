#pragma once

#include <stdio.h>
#include "WinSock2.h"
#include "Ws2tcpip.h"
#include <windows.h>
#include "../../debug_print/debug_print.h"


// Link with -lws2_32 when compiling


// Downloads data from the specified URI, using sockets
// Returns pointer to data, memory is allocated by the function.
// data_size receives the size of the data in bytes.
unsigned char* download_data(char* uri, int *data_size)
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

    *data_size = j;
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


// Downloads the data from the URI specified in arg1.
// No file is dropped, the data is read directly from the socket.
//
// data_size receives the size of the data in bytes.
unsigned char* download_socket(char *arg1, int *data_size) {
	DEBUG_PRINT(("Downloading data from url via sockets...\n"));
		
	return download_data(arg1, data_size);
}
