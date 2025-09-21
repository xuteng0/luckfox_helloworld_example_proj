#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <time.h>
#include <string.h>

void print_system_info() {
    struct utsname system_info;
    struct sysinfo sys_info;
    FILE *fp;
    char buffer[256];
    
    printf("\n=== SYSTEM INFORMATION ===\n");
    
    // Get system name, kernel version, etc.
    if (uname(&system_info) == 0) {
        printf("System Name:     %s\n", system_info.sysname);
        printf("Node Name:       %s\n", system_info.nodename);
        printf("Kernel Release:  %s\n", system_info.release);
        printf("Kernel Version:  %s\n", system_info.version);
        printf("Machine:         %s\n", system_info.machine);
    }
    
    // Get system uptime and load
    if (sysinfo(&sys_info) == 0) {
        printf("Uptime:          %ld seconds (%ld days, %ld hours, %ld minutes)\n", 
               sys_info.uptime,
               sys_info.uptime / 86400,
               (sys_info.uptime % 86400) / 3600,
               (sys_info.uptime % 3600) / 60);
        
        printf("Load Average:    %.2f, %.2f, %.2f\n",
               sys_info.loads[0] / 65536.0,
               sys_info.loads[1] / 65536.0,
               sys_info.loads[2] / 65536.0);
        
        printf("Total RAM:       %lu MB\n", sys_info.totalram / (1024 * 1024));
        printf("Free RAM:        %lu MB\n", sys_info.freeram / (1024 * 1024));
        printf("Process Count:   %d\n", sys_info.procs);
    }
    
    // Try to get CPU info
    fp = fopen("/proc/cpuinfo", "r");
    if (fp != NULL) {
        printf("\n=== CPU INFORMATION ===\n");
        while (fgets(buffer, sizeof(buffer), fp)) {
            if (strncmp(buffer, "processor", 9) == 0 ||
                strncmp(buffer, "model name", 10) == 0 ||
                strncmp(buffer, "cpu MHz", 7) == 0 ||
                strncmp(buffer, "BogoMIPS", 8) == 0 ||
                strncmp(buffer, "Features", 8) == 0) {
                printf("%s", buffer);
            }
        }
        fclose(fp);
    }
    
    // Try to get memory info
    fp = fopen("/proc/meminfo", "r");
    if (fp != NULL) {
        printf("\n=== MEMORY INFORMATION ===\n");
        while (fgets(buffer, sizeof(buffer), fp)) {
            if (strncmp(buffer, "MemTotal", 8) == 0 ||
                strncmp(buffer, "MemFree", 7) == 0 ||
                strncmp(buffer, "MemAvailable", 12) == 0 ||
                strncmp(buffer, "Buffers", 7) == 0 ||
                strncmp(buffer, "Cached", 6) == 0) {
                printf("%s", buffer);
            }
        }
        fclose(fp);
    }
    
    // Get current time
    time_t current_time = time(NULL);
    printf("\n=== TIME INFORMATION ===\n");
    printf("Current Time:    %s", ctime(&current_time));
    
    printf("=============================\n\n");
}

int main(int argc, char *argv[])
{
    printf("Hello, World! This is a simple hello world application for Luckfox Pico.\n");
    printf("Program started with %d arguments:\n", argc);
    
    for (int i = 0; i < argc; i++) {
        printf("  argv[%d]: %s\n", i, argv[i]);
    }
    
    // Print system information
    print_system_info();
    
    printf("Application is running successfully!\n");
    
    return 0;
}
