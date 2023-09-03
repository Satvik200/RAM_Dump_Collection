#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>
#include <stdio.h>

int main()
{
    HANDLE hFile = CreateFile(_T("memory.dmp"), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        printf("Failed to create dump file. Error: %d\n", GetLastError());
        return 1;
    }

    FILE* fDump = fopen("memory.txt", "w");
    if (fDump == NULL) {
        printf("Failed to create dump file. Error: %d\n", GetLastError());
        CloseHandle(hFile);
        return 1;
    }

    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    DWORD pageSize = sysInfo.dwPageSize;

    MEMORY_BASIC_INFORMATION memInfo;
    for (LPVOID addr = 0; addr < sysInfo.lpMaximumApplicationAddress; addr = (LPBYTE)memInfo.BaseAddress + memInfo.RegionSize) {
        if (VirtualQuery(addr, &memInfo, sizeof(MEMORY_BASIC_INFORMATION)) == 0) {
            printf("VirtualQuery failed. Error: %d\n", GetLastError());
            CloseHandle(hFile);
            fclose(fDump);
            return 1;
        }

        if (memInfo.State == MEM_COMMIT && memInfo.Protect != PAGE_NOACCESS) {
            DWORD bytesRead = 0;
            LPVOID buffer = VirtualAlloc(NULL, memInfo.RegionSize, MEM_COMMIT, PAGE_READWRITE);
            if (buffer == NULL) {
                printf("VirtualAlloc failed. Error: %d\n", GetLastError());
                CloseHandle(hFile);
                fclose(fDump);
                return 1;
            }
            if (!ReadProcessMemory(GetCurrentProcess(), memInfo.BaseAddress, buffer, memInfo.RegionSize, &bytesRead)) {
                printf("ReadProcessMemory failed. Error: %d\n", GetLastError());
                CloseHandle(hFile);
                VirtualFree(buffer, 0, MEM_RELEASE);
                fclose(fDump);
                return 1;
            }

            DWORD bytesWritten = 0;
            if (!WriteFile(hFile, buffer, memInfo.RegionSize, &bytesWritten, NULL)) {
                printf("WriteFile failed. Error: %d\n", GetLastError());
                CloseHandle(hFile);
                VirtualFree(buffer, 0, MEM_RELEASE);
                fclose(fDump);
                return 1;
            }

            fprintf(fDump, "Address: %p, Size: %lu\n", memInfo.BaseAddress, memInfo.RegionSize);
            for (DWORD i = 0; i < memInfo.RegionSize; i += sizeof(DWORD)) {
                DWORD* p = (DWORD*)((LPBYTE)buffer + i);
                fprintf(fDump, "%08X ", *p);
                if ((i + sizeof(DWORD)) % 16 == 0) fprintf(fDump, "\n");
            }
            fprintf(fDump, "\n");

            VirtualFree(buffer, 0, MEM_RELEASE);
        }
    }

    printf("Memory dump created successfully.\n");
    CloseHandle(hFile);
    fclose(fDump);
    return 0;
}
