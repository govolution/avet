#pragma once

#include <stdio.h>
#include <string.h>
#include "winsock2.h"
#include "ws2tcpip.h"
#include "../debug_print/debug_print.h"


// Link with -lws2_32 when compiling


// Downloads data from the specified URI, using sockets
// Returns pointer to data, memory is allocated by the function.
// data_size receives the size of the data in bytes.
unsigned char *download_data(char *uri, int *data_size) {
	WSADATA wsa;
	SOCKET s;
	struct addrinfo hints;
	memset(&hints, 0, sizeof(struct addrinfo));
	struct addrinfo* ai;

	// Prepare URI strings
	// URI is expected to be of format http://myserver.net/filename:port

    // Remove http prefix, if set
    char *uri_short = (char *) malloc(strlen(uri));
    if(strstr(uri, "http://") == uri) {
        strcpy(uri_short, uri + 7);
    } else {
        strcpy(uri_short, uri);
    }

    // Extract hostname
	char server_hostname[256];
    strcpy(server_hostname, uri_short);
    *strchr(server_hostname, '/') = '\0';

	// Truncate after last '/' and before ':'  to get the file name
	char server_filename[256];
	strcpy(server_filename, strrchr(uri_short, '/') + 1);
	// No ':' means that no port is specified. Avoid access violation.
	if(strrchr(server_filename, ':') != NULL) {
		*strrchr(server_filename, ':') = '\0';
	}

	// Truncate after last ':' to get the server port as string
	char server_port_string[256];

	if(strrchr(uri_short, ':') != NULL) {
		strcpy(server_port_string, strrchr(uri_short, ':') + 1);
	// No ':' means that no port is specified. In this case, assume port 80.
	} else {
		strcpy(server_port_string, "80");
	}

	DEBUG_PRINT("Attempting to download data from target into memory via HTTP request...\n");
	DEBUG_PRINT("\tTarget host:\t%s\n", server_hostname);
	DEBUG_PRINT("\tTarget port:\t%s\n", server_port_string);
	DEBUG_PRINT("\tRequested file:\t%s\n", server_filename);

	// Initialize WSA
	if(WSAStartup(MAKEWORD(2, 0), &wsa) != 0) {
		DEBUG_PRINT("WSA initialization failed!\n");
		return NULL;
	}

	// Init server data
	// Get server IP address
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	if(getaddrinfo(server_hostname, server_port_string, &hints, &ai) != 0) {
		DEBUG_PRINT("getaddrinfo failed!\n");
		return NULL;
	}

	// Create socket
	if((s = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol)) == INVALID_SOCKET) {
		DEBUG_PRINT("Socket creation failed!\n");
		return NULL;
	}

	// Connect to server
	if(connect(s, ai->ai_addr, ai->ai_addrlen) < 0) {
		DEBUG_PRINT("Connection to server failed!\n");
		return NULL;
	}

	// Free address info struct, no longer needed
	freeaddrinfo(ai);

	// Assemble and send HTTP GET request
	char request[512];
	sprintf(request, "GET %s HTTP/1.1\r\n\r\n", server_filename);
	sprintf(request + strlen(request), "Host: %s\r\n\r\n", server_hostname);

	DEBUG_PRINT("Sending request:\n%s\n", request);

	if(send(s, request, strlen(request), 0) < strlen(request)) {
		DEBUG_PRINT("Sending HTTP GET request failed!\n");
		return NULL;
	}

	// Parse response header and extract content length
	char response[512];

	while (strcmp(response, "\r\n")) {
		for (int i = 0; strcmp(response + i - 2, "\r\n"); i++) {
			recv(s, response + i, 1, 0); response[i + 1] = '\0';
		}

		if (strstr(response, "Content-Length:") == response) {
			*strchr(response, '\r') = '\0';
			*data_size = atoi(strchr(response, ' ') + 1);
		}
	}

	// Allocate memory of respective size for received data
	unsigned char *data = (unsigned char *) malloc(*data_size);

	unsigned char current_byte;
	for (int i = 0; i < *data_size; i++) {
		recv(s, &current_byte, 1, 0);
		data[i] = current_byte;
	}

	// Cleanup
	closesocket(s);
	WSACleanup();

	DEBUG_PRINT("Data received, %d bytes.\n", *data_size);

	return data;
}


// Downloads the data from the URI specified in arg1.
// No file is dropped, the data is read directly from the socket.
//
// data_size receives the size of the data in bytes.
unsigned char* download_socket(char *arg1, int *data_size) {
	DEBUG_PRINT("Downloading data from url via sockets...\n");

	return download_data(arg1, data_size);
}
