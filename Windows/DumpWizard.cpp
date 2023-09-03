// #define WIN32_LEAN_AND_MEAN
// #include <windows.h>
// #include <tchar.h>
// #include <stdio.h>

// int main()
// {
//     HANDLE hFile = CreateFile(_T("memory.dmp"), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
//     if (hFile == INVALID_HANDLE_VALUE) {
//         printf("Failed to create dump file. Error: %d\n", GetLastError());
//         return 1;
//     }

//     HANDLE hTxtFile = CreateFile(_T("memory.txt"), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
//     if (hTxtFile == INVALID_HANDLE_VALUE) {
//         printf("Failed to create txt file. Error: %d\n", GetLastError());
//         CloseHandle(hFile);
//         return 1;
//     }

//     SYSTEM_INFO sysInfo;
//     GetSystemInfo(&sysInfo);
//     DWORD pageSize = sysInfo.dwPageSize;

//     MEMORY_BASIC_INFORMATION memInfo;
//     for (LPVOID addr = 0; addr < sysInfo.lpMaximumApplicationAddress; addr = (LPBYTE)memInfo.BaseAddress + memInfo.RegionSize) {
//         if (VirtualQuery(addr, &memInfo, sizeof(MEMORY_BASIC_INFORMATION)) == 0) {
//             printf("VirtualQuery failed. Error: %d\n", GetLastError());
//             CloseHandle(hFile);
//             CloseHandle(hTxtFile);
//             return 1;
//         }

//         if (memInfo.State == MEM_COMMIT && memInfo.Protect != PAGE_NOACCESS) {
//             DWORD bytesRead = 0;
//             LPVOID buffer = VirtualAlloc(NULL, memInfo.RegionSize, MEM_COMMIT, PAGE_READWRITE);
//             if (buffer == NULL) {
//                 printf("VirtualAlloc failed. Error: %d\n", GetLastError());
//                 CloseHandle(hFile);
//                 CloseHandle(hTxtFile);
//                 return 1;
//             }
//             if (!ReadProcessMemory(GetCurrentProcess(), memInfo.BaseAddress, buffer, memInfo.RegionSize, &bytesRead)) {
//                 printf("ReadProcessMemory failed. Error: %d\n", GetLastError());
//                 CloseHandle(hFile);
//                 CloseHandle(hTxtFile);
//                 VirtualFree(buffer, 0, MEM_RELEASE);
//                 return 1;
//             }

//             DWORD bytesWritten = 0;
//             if (!WriteFile(hFile, buffer, memInfo.RegionSize, &bytesWritten, NULL)) {
//                 printf("WriteFile failed. Error: %d\n", GetLastError());
//                 CloseHandle(hFile);
//                 CloseHandle(hTxtFile);
//                 VirtualFree(buffer, 0, MEM_RELEASE);
//                 return 1;
//             }

//             if (!WriteFile(hTxtFile, buffer, memInfo.RegionSize, &bytesWritten, NULL)) {
//                 printf("WriteFile failed. Error: %d\n", GetLastError());
//                 CloseHandle(hFile);
//                 CloseHandle(hTxtFile);
//                 VirtualFree(buffer, 0, MEM_RELEASE);
//                 return 1;
//             }

//             VirtualFree(buffer, 0, MEM_RELEASE);
//         }
//     }

//     printf("Memory dump created successfully.\n");
//     CloseHandle(hFile);
//     CloseHandle(hTxtFile);
//     return 0;
// }



// Preprocessor directive to define the macro WIN32_LEAN_AND_MEAN to speed up the build process
#define WIN32_LEAN_AND_MEAN

// Include necessary header files
#include <windows.h>    // For Windows APIs
#include <tchar.h>      // For Unicode support
#include <stdio.h>      // For file I/O

int main()
{
    // Create a file handle to write memory dump to file "memory.dmp"
    HANDLE hFile = CreateFile(_T("memory.dmp"), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    // Check if the handle is valid, if not print error message and return 1 (failure)
    if (hFile == INVALID_HANDLE_VALUE) {
        printf("Failed to create dump file. Error: %d\n", GetLastError());
        return 1;
    }
    
    // Create a file pointer to write dump contents in human-readable format to file "memory.txt"
    FILE* fDump = fopen("memory.txt", "w");

    // Check if file pointer is valid, if not print error message, close file handle and return 1 (failure)
    if (fDump == NULL) {
        printf("Failed to create dump file. Error: %d\n", GetLastError());
        CloseHandle(hFile);
        return 1;
    }

    // Get system information, including the page size
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    DWORD pageSize = sysInfo.dwPageSize;

    // Define a MEMORY_BASIC_INFORMATION structure to store information about the memory regions
    MEMORY_BASIC_INFORMATION memInfo;

    // Iterate through all memory regions of the current process
    for (LPVOID addr = 0; addr < sysInfo.lpMaximumApplicationAddress; addr = (LPBYTE)memInfo.BaseAddress + memInfo.RegionSize) {

        // Query the current memory region for its properties
        if (VirtualQuery(addr, &memInfo, sizeof(MEMORY_BASIC_INFORMATION)) == 0) {
            printf("VirtualQuery failed. Error: %d\n", GetLastError());
            CloseHandle(hFile);
            fclose(fDump);
            return 1;
        }

        // Check if the memory region is committed and accessible
        if (memInfo.State == MEM_COMMIT && memInfo.Protect != PAGE_NOACCESS) {

            // Allocate memory for the memory dump buffer
            DWORD bytesRead = 0;
            LPVOID buffer = VirtualAlloc(NULL, memInfo.RegionSize, MEM_COMMIT, PAGE_READWRITE);

            // Check if the allocation was successful, if not print error message, close file handle and file pointer and return 1 (failure)
            if (buffer == NULL) {
                printf("VirtualAlloc failed. Error: %d\n", GetLastError());
                CloseHandle(hFile);
                fclose(fDump);
                return 1;
            }

            // Read the memory region contents into the buffer
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
