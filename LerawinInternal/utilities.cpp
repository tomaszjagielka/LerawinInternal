#include <Windows.h>
#include <Psapi.h>
#include <cstdint>
#include <vector>

#include "dt_recv.h"
#include "client_class.h"

#include "variables.h"
#include "entity.h"
#include "utilities.h"

typedef void *(__cdecl *tCreateInterface)(const char *name, int *return_code); // Function prototype (?).
void *GetInterface(const char *dll_name, const char *interface_name)
{
    // Maybe needs error checking.

    tCreateInterface CreateInterface = (tCreateInterface)GetProcAddress(GetModuleHandle(dll_name), "CreateInterface");

    //int return_code = 0;
    void *csgo_interface = CreateInterface(interface_name, 0);

    return csgo_interface;
}

intptr_t GetOffset(RecvTable *table, const char *table_name, const char *netvar_name)
{
    for (int i = 0; i < table->m_nProps; i++)
    {
        RecvProp prop = table->m_pProps[i];

        if (!_stricmp(prop.m_pVarName, netvar_name))
        {
            return prop.m_Offset;
        }

        if (prop.m_pDataTable)
        {
            intptr_t offset = GetOffset(prop.m_pDataTable, table_name, netvar_name);

            if (offset)
            {
                return offset + prop.m_Offset;
            }
        }
    }
    return 0;
}

intptr_t GetNetVarOffset(ClientClass *client_class, const char *table_name, const char *netvar_name, int offset)
{
    ClientClass *curr_node = client_class;

    for (auto curr_node = client_class; curr_node; curr_node = curr_node->m_pNext)
    {
        if (!_stricmp(table_name, curr_node->m_pRecvTable->m_pNetTableName))
        {
            return GetOffset(curr_node->m_pRecvTable, table_name, netvar_name) + offset;
        }
    }

    return 0;
}

uintptr_t PatternScan(uintptr_t module_address, const char *signature, int offset, int extra, bool relative)
{
    static auto PatternToByte = [](const char *pattern)
    {
        std::vector<int>bytes = std::vector<int>{};
        char *const     start = const_cast<char *>(pattern);
        char *const     end = const_cast<char *>(pattern) + strlen(pattern);

        for (char *current = start; current < end; ++current)
        {
            // Replace question marks with -1 (0xFFFFFFFF), which later then are filtered out.
            if (*current == '?')
            {
                ++current;
                if (*current == '?')
                    ++current;
                bytes.push_back(-1);
            }
            else { bytes.push_back(strtoul(current, &current, 16)); }
        }
        return bytes;
    };

    const PIMAGE_DOS_HEADER dos_header = (PIMAGE_DOS_HEADER)module_address;
    const PIMAGE_NT_HEADERS nt_header = (PIMAGE_NT_HEADERS)((std::uint8_t *)module_address + dos_header->e_lfanew);

    const uintptr_t size_of_image = nt_header->OptionalHeader.SizeOfImage;
    std::vector<int>pattern_bytes = PatternToByte(signature);
    uint8_t *const scan_bytes = reinterpret_cast<std::uint8_t *>(module_address);

    const size_t size = pattern_bytes.size();
    int *const data = pattern_bytes.data();

    for (unsigned long i = 0ul; i < size_of_image - size; ++i)
    {
        bool found = true;
        for (unsigned long j = 0ul; j < size; ++j)
        {
            if (scan_bytes[i + j] != data[j] && data[j] != -1)
            {
                found = false;
                break;
            }
        }
        if (found) {
            uintptr_t address = *(uintptr_t *)&scan_bytes[i + offset];
            if (extra)
                address += extra;
            if (relative)
                address -= module_address;
            return address;
        }
    }
    return NULL;
}

//uintptr_t FindPattern(HMODULE module, const char* pattern, const char* mask, int offsets = 0, int extra = 0)
//{
//	//Get all module related information
//	MODULEINFO mInfo = GetModuleInfo(module);
//
//	//Assign our base and module size
//	uintptr_t base = (uintptr_t)mInfo.lpBaseOfDll;
//	uintptr_t size = (uintptr_t)mInfo.SizeOfImage;
//
//	//Get length for our mask, this will allow us to loop through our array
//	uintptr_t patternLength = (uintptr_t)strlen(mask);
//
//	for (uintptr_t i = 0; i < size - patternLength; i++)
//	{
//		bool found = true;
//		for (uintptr_t j = 0; j < patternLength; j++)
//		{
//			//if we have a ? in our mask then we have true by default,
//			//or if the bytes match then we keep searching until finding it or not
//			found &= mask[j] == '?' || pattern[j] == *(char*)(base + i + j);
//		}
//
//		//found = true, our entire pattern was found
//		if (found)
//		{
//			return base + i;
//		}
//	}
//	return NULL;
//}