#include <stdio.h>
#include <string.h>
#include "user.h"

#include <limits.h>
#include <process.h>
#include <stdlib.h>

#include "system_calls.h"

void user_shell() {
    char command[256];
    printf("Type 'help' for a list of commands.\n");

    while (1) {
        printf("User> ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0;

        if (strcmp(command, "exit") == 0) break;

        handle_user_command(command);
    }
}

void handle_user_command(const char *command) {
    if (strcmp(command, "ls") == 0) {
        sys_call_list_files();
    } else if (strncmp(command, "touch ", 6) == 0) {
        sys_call_create_file(command + 6);
    } else if (strcmp(command, "help") == 0) {
        printf("Available commands:\n");
        printf("  ls            - List all files\n");
        printf("  touch <file>  - Create a file\n");
        printf("  rm <file>     - Delete a file\n");
        printf("  mkdir <dir>   - Create a directory\n");
        printf("  rmdir <dir>   - Remove a directory\n");
        printf("  ps            - List all processes\n");
        printf("  kill <pid>    - Terminate a process\n");
        printf("  exit          - Exit the OS simulation\n");
        printf("  help          - Display this help message\n");
    } else if (strncmp(command, "run ", 4) == 0) {
        // Extract program name
        const char *program_name = command + 4;

        // Create a new process
        static int pid_counter = 1;  // Simple PID generator
        create_process(pid_counter++, 10); // Default 10 cycles for a new program
        printf("Running program: %s (PID=%d)\n", program_name, pid_counter - 1);
    } else if (strcmp(command, "ps") == 0) {
        // Display all running processes
        Process *curr = process_queue.head;
        printf("PID\tState\t\tTime Remaining\n");
        while (curr) {
            printf("%d\t%s\t\t%d\n", curr->pid,
                   curr->state == READY ? "Ready" :
                   curr->state == RUNNING ? "Running" :
                   curr->state == BLOCKED ? "Blocked" : "Terminated",
                   curr->time_remaining);
            curr = curr->next;
        }
    } else if (strncmp(command, "kill ", 5) == 0) {
        // Terminate a process by PID
        char *endptr;
        const long pid = strtol(command + 5, &endptr, 10);
        if (*endptr == '\0' && pid <= INT_MAX && pid >= INT_MIN) {
            terminate_process(pid);
        } else {
            printf("Invalid PID: %s\n", command + 5);
        }
    } else {
        printf("Unknown command: %s\n", command);
    }
}

