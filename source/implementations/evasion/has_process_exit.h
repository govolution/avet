// look for process, exit if process exist
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <tchar.h>
#include <process.h>
#include <windows.h>
#include <tlhelp32.h>
#include "../debug_print/debug_print.h"

using namespace std;

BOOL GetProcessList(char *arg1);
BOOL TerminateBlacklistedProcess(DWORD dwProcessId, UINT uExitCode);

void has_process_exit( char *arg1 )
{
    DEBUG_PRINT("Applying has_processes_exit technique.\n");
    DEBUG_PRINT("Looking for process %s and exit if found.\n", arg1);
    GetProcessList( arg1 );
}

BOOL GetProcessList(char *arg1 )
{
  DEBUG_PRINT("GetProcessList\n");
  HANDLE hProcessSnap;
  HANDLE hProcess;
  PROCESSENTRY32 pe32;
  DWORD dwPriorityClass;

  //Blacklisted processes
  LPSTR ProcessName = arg1;

  // Take a snapshot of processes
  hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
  if( hProcessSnap == INVALID_HANDLE_VALUE )
  {
    printf("exit\n");
    return( FALSE );
  }

  pe32.dwSize = sizeof( PROCESSENTRY32 );

  if( !Process32First( hProcessSnap, &pe32 ) )
  {
    CloseHandle( hProcessSnap );
    return( FALSE );
  }

  do
  {
    string str(pe32.szExeFile);


    if(str == ProcessName)
    {
        cout << "[*] process found, exit: " << (ProcessName) << endl;
        return (TRUE);
    }
    
  } while( Process32Next( hProcessSnap, &pe32 ) );

  CloseHandle( hProcessSnap );
  return( TRUE );
}
