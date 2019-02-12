// Modified, but basically taken from https://github.com/hasherezade/demos
//
// Big credits go to hasherezade!


#pragma once


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


IMAGE_DATA_DIRECTORY* get_pe_directory64(PVOID pe_buffer, DWORD dir_id)
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
	
	if (((PVOID) ((DWORD64) peDir->VirtualAddress)) == NULL) {			
		return NULL;
	}	
	
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


bool has_relocations64(BYTE *pe_buffer)
{
    IMAGE_DATA_DIRECTORY* relocDir = get_pe_directory64(pe_buffer, IMAGE_DIRECTORY_ENTRY_BASERELOC);
    if (relocDir == NULL) {
        return false;
    }
    return true;
}


bool apply_reloc_block64(BASE_RELOCATION_ENTRY *block, SIZE_T entriesNum, DWORD page, ULONGLONG oldBase, ULONGLONG newBase, PVOID modulePtr)
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
			case RELOC_64BIT_FIELD:
				relocateAddr64 = (ULONGLONG*) ((ULONG_PTR) modulePtr + page + offset);
				(*relocateAddr64) = ((ULONGLONG) (*relocateAddr64)) - oldBase + newBase;
				entry = (BASE_RELOCATION_ENTRY*)((ULONG_PTR) entry + sizeof(WORD));	
				break;			
			default:
				DEBUG_PRINT("Not supported relocations format at %d: %d\n", (int) i, type);
				return false;
		}								
				
	}
	//printf("[+] Applied %d relocations\n", (int) i);
	return true;		
}


bool apply_relocations64(ULONGLONG newBase, ULONGLONG oldBase, PVOID modulePtr)
{
    IMAGE_DATA_DIRECTORY* relocDir = get_pe_directory64(modulePtr, IMAGE_DIRECTORY_ENTRY_BASERELOC);
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
		
		if ((((ULONGLONG*) ((ULONGLONG) reloc->VirtualAddress)) == NULL) || (reloc->SizeOfBlock == 0)) {
			continue;
		}	
				
        //printf("RelocBlock: %x %x\n", reloc->VirtualAddress, reloc->SizeOfBlock);
        
        size_t entriesNum = (reloc->SizeOfBlock - 2 * sizeof(DWORD))  / sizeof(WORD);
        DWORD page = reloc->VirtualAddress;

        BASE_RELOCATION_ENTRY* block = (BASE_RELOCATION_ENTRY*)((ULONG_PTR) reloc + sizeof(DWORD) + sizeof(DWORD));
        if (apply_reloc_block64(block, entriesNum, page, oldBase, newBase, modulePtr) == false) {
            return false;
        }
    }
    return true;
}

