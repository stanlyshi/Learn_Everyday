
//or---------------------------------------------------------------------------------------
//or---------------------------------------------------------------------------------------
//or---------------------------------------------------------------------------------------



#include <sys/resource.h>
#include <errno.h>

int foo() {
    errno = 0;
    struct rusage* memory = malloc(sizeof(struct rusage));
    getrusage(RUSAGE_SELF, memory);
    if(errno == EFAULT)
        printf("Error: EFAULT\n");
    else if(errno == EINVAL)
        printf("Error: EINVAL\n");
    printf("Usage: %ld\n", memory->ru_ixrss);
    printf("Usage: %ld\n", memory->ru_isrss);
    printf("Usage: %ld\n", memory->ru_idrss);
    printf("Max: %ld\n", memory->ru_maxrss);
}


//or---------------------------------------------------------------------------------------
//or---------------------------------------------------------------------------------------
//or---------------------------------------------------------------------------------------



#include <sys/resource.h>
#include <stdio.h>

int main() {
  struct rusage r_usage;
  getrusage(RUSAGE_SELF,&r_usage);
  // Print the maximum resident set size used (in kilobytes).
  printf("Memory usage: %ld kilobytes\n",r_usage.ru_maxrss);
  return 0;
}

//or---------------------------------------------------------------------------------------
//or---------------------------------------------------------------------------------------
//or---------------------------------------------------------------------------------------


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
 * Measures the current (and peak) resident and virtual memories
 * usage of your linux C process, in kB
 */
void getMemory(
    int* currRealMem, int* peakRealMem,
    int* currVirtMem, int* peakVirtMem) {

    // stores each word in status file
    char buffer[1024] = "";

    // linux file contains this-process info
    FILE* file = fopen("/proc/self/status", "r");

    // read the entire file
    while (fscanf(file, " %1023s", buffer) == 1) {

        if (strcmp(buffer, "VmRSS:") == 0) {
            fscanf(file, " %d", currRealMem);
        }
        if (strcmp(buffer, "VmHWM:") == 0) {
            fscanf(file, " %d", peakRealMem);
        }
        if (strcmp(buffer, "VmSize:") == 0) {
            fscanf(file, " %d", currVirtMem);
        }
        if (strcmp(buffer, "VmPeak:") == 0) {
            fscanf(file, " %d", peakVirtMem);
        }
    }
    fclose(file);
}


