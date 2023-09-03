# RAM Dump Collection Tool

### Introduction
The RAM Dump Collection Tool is a powerful utility designed to facilitate the collection of RAM (Random Access Memory) dumps from Windows-based systems. RAM dumps are essential for digital forensics and incident response activities, aiding in the analysis of system memory for suspicious or malicious activities. This tool simplifies the process of collecting RAM dumps, making it accessible to both cybersecurity professionals and forensic investigators.

### Requirements
Before using the RAM Dump Collection Tool, ensure that you meet the following requirements:
1. A Windows-based system (Windows 7 or later) where you want to collect RAM dumps.
2. Sufficient permissions to access and collect memory data.
3. Administrative privileges may be required for certain operations.

### Dump-Wizard
This program creates a memory dump of the current process and writes it to a file. It does this by iterating over the virtual memory regions of the process using the VirtualQuery function, and for each region that is committed and readable, it allocates a buffer, reads the memory into the buffer using the ReadProcessMemory function, and writes the buffer to a file using the WriteFile function. Additionally, the program writes the contents of the memory dump to a text file, along with the address and size of each memory region.

### Dump-Analyzer
This program analyzes a memory dump file generated from Dump-Wizard and writes the analysis to a text file. It uses the Windows API to open the memory dump file, allocate memory to read it into, and then read the contents of the file into memory. The program then analyzes the memory dump, writing the results to a text file. The output includes the dump file name, the file size, and the contents of the file, represented as 8-digit hexadecimal values. The program is designed to run on Windows operating systems and requires the Microsoft Visual C++ compiler to build.
