#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

int main() {
    // Open a file called memory.dmp with read, write, and create options and set file permissions for user to read and write
    int fd = open("memory.dmp", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    
    // If opening the file failed, print an error message and return 1
    if (fd == -1) {
        printf("Failed to create dump file.\n");
        return 1;
    }

    // Open a file called memory.txt with write permission 
    FILE* fDump = fopen("memory.txt", "w");
    
    // If opening the file failed, print an error message, close the previously opened file, and return 1
    if (fDump == NULL) {
        printf("Failed to create dump file.\n");
        close(fd);
        return 1;
    }

    // Get the page size of the system
    long page_size = sysconf(_SC_PAGESIZE);

    // Initialize a void pointer to NULL and a struct stat object to store the status of a file
    void* addr = 0;
    struct stat st;
    
    // Get the status of the file /proc/self/maps and store it in the struct stat object
    if (stat("/proc/self/maps", &st) == -1) {
        printf("Failed to get memory map. \n");
        close(fd);
        fclose(fDump);
        return 1;
    }

    // Allocate memory for a char buffer that is the same size as the file status struct plus 1
    char* buffer = malloc(st.st_size + 1);
    
    // If allocation of the buffer fails, print an error message, close the previously opened files, free the buffer memory, and return 1
    if (buffer == NULL) {
        printf("Failed to allocate buffer. \n");
        close(fd);
        fclose(fDump);
        return 1;
    }

    // Open the file /proc/self/maps with read permission
    FILE* maps = fopen("/proc/self/maps", "r");
    
    // If opening the file fails, print an error message, close the previously opened files, free the buffer memory, and return 1
    if (maps == NULL) {
        printf("Failed to open memory map. \n");
        close(fd);
        fclose(fDump);
        free(buffer);
        return 1;
    }

    // Read each line from the file /proc/self/maps
    while (fgets(buffer, st.st_size, maps)) {
        // Initialize variables to store information from each line in the file
        unsigned long long start, end, inode, offset;
        char perm[5], dev[6], pathname[1024];

        // Extract information from the line in the file and store it in the respective variables
        if (sscanf(buffer, "%llx-%llx %4s %llx %5s %lld %s", &start, &end, perm, &offset, dev, &inode, pathname) == 7) {
            // If the memory permission is readable and private, proceed
            if (perm[0] == 'r' && perm[3] == 'p') {
                // Calculate the size of the memory block
                size_t size = end - start;
                // If the size is greater than 0, map the memory block to the file memory.dmp
                if (size > 0) {
                    void* ptr = mmap(NULL, size, PROT_READ, MAP_PRIVATE, 0, start);
                    if (ptr == MAP_FAILED) {
                        printf("Failed to map memory.\n");
                        continue;
                    }
                    // Write the memory block to the file memory.dmp and the file memory.txt
                    if (write(fd, ptr, size) != size || fprintf(fDump, "%llx-%llx %4s %llx %5s %lld %s\n", start, end, perm, offset, dev, inode, pathname) < 0) {
                        printf("Failed to dump memory.\n");
                    }
                    // Unmap the memory block from the process's virtual address space
                    munmap(ptr, size);
                }
            }
        }
    }
    // Close the files and free the buffer memory
    close(fd);
    fclose(fDump);
    free(buffer);
    return 0;
}