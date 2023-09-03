#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>
#include <stdio.h>

int main()
{
const char* dumpFileName = "memory.dmp";
const char* analysisFileName = "analysis.txt";

// Open the dump file
FILE* fDump = fopen(dumpFileName, "rb");
if (fDump == NULL) {
    printf("Failed to open dump file '%s'. Error: %d\n", dumpFileName, GetLastError());
    return 1;
}

// Create the analysis file
FILE* fAnalysis = fopen(analysisFileName, "w");
if (fAnalysis == NULL) {
    printf("Failed to create analysis file '%s'. Error: %d\n", analysisFileName, GetLastError());
    fclose(fDump);
    return 1;
}

// Get the size of the dump file
DWORD fileSize = 0;
fseek(fDump, 0, SEEK_END);
fileSize = ftell(fDump);
fseek(fDump, 0, SEEK_SET);

// Allocate memory to read the dump file into
LPVOID buffer = VirtualAlloc(NULL, fileSize, MEM_COMMIT, PAGE_READWRITE);
if (buffer == NULL) {
    printf("VirtualAlloc failed. Error: %d\n", GetLastError());
    fclose(fDump);
    fclose(fAnalysis);
    return 1;
}

// Read the dump file into memory
DWORD bytesRead = fread(buffer, 1, fileSize, fDump);
if (bytesRead != fileSize) {
    printf("Failed to read dump file '%s'. Error: %d\n", dumpFileName, GetLastError());
    VirtualFree(buffer, 0, MEM_RELEASE);
    fclose(fDump);
    fclose(fAnalysis);
    return 1;
}

// Write the dump file analysis to the analysis file
fprintf(fAnalysis, "Memory dump analysis for file: %s\n", dumpFileName);
fprintf(fAnalysis, "Dump file size: %u bytes\n", fileSize);
fprintf(fAnalysis, "Dump file contents:\n\n");
for (DWORD i = 0; i < fileSize; i += sizeof(DWORD)) {
    DWORD* p = (DWORD*)((LPBYTE)buffer + i);
    fprintf(fAnalysis, "%08X ", *p);
    if ((i + sizeof(DWORD)) % 16 == 0) fprintf(fAnalysis, "\n");
}

printf("Dump analysis completed successfully. Results written to file '%s'.\n", analysisFileName);
VirtualFree(buffer, 0, MEM_RELEASE);
fclose(fDump);
fclose(fAnalysis);
return 0;
}