#include <stdio.h>
#include <string.h>
#include "user.h"

#include <limits.h>
#include <process.h>
#include <stdlib.h>
#include <unistd.h>

#include "system_calls.h"

extern char project_root[1024];

void user_shell() {
    char command[256];
    char cwd[1024]; // Buffer to store the current working directory

    while (1) {
        // Get the current working directory
        if (getcwd(cwd, sizeof(cwd)) == NULL) {
            perror("Failed to get current working directory");
            break;
        }

        printf("\nUser ");

        // Compare cwd with project_root and adjust the prompt
        if (strcmp(cwd, project_root) == 0) {
            printf("~> ");
        } else if (strncmp(cwd, project_root, strlen(project_root)) == 0) {
            printf("~%s> ", cwd + strlen(project_root)); // Show relative path from project root
        } else {
            printf("%s> ", cwd); // Show full path for directories outside the project root
        }

        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0; // Remove newline character

        if (strcmp(command, "exit") == 0) break;

        handle_user_command(command);
    }
}

void handle_user_command(const char *command) {
    if (strcmp(command, "help") == 0) {
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
        static int pid_counter = 1; // Simple PID generator
        create_process(pid_counter++, 10); // Default 10 cycles for a new program
        printf("Running program: %s (PID=%d)\n", program_name, pid_counter - 1);
    } else if (strcmp(command, "ps") == 0) {
        // Display all running processes
        Process *curr = process_queue.head;
        printf("PID\tState\t\tTime Remaining\n");
        while (curr) {
            printf("%d\t%s\t\t%d\n", curr->pid,
                   curr->state == READY
                       ? "Ready"
                       : curr->state == RUNNING
                             ? "Running"
                             : curr->state == BLOCKED
                                   ? "Blocked"
                                   : "Terminated",
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
    } else if (strncmp(command, "touch ", 6) == 0) {
        sys_create_file(command + 6);
    } else if (strncmp(command, "echo ", 5) == 0) {
        char *message_start = strchr(command + 5, '"'); // Find the first quote
        if (message_start) {
            char *message_end = strchr(message_start + 1, '"'); // Find the closing quote
            if (message_end) {
                *message_end = '\0'; // Null-terminate the message
                char *redirect = strstr(message_end + 1, ">"); // Find >
                if (redirect) {
                    char *filename = redirect + 2; // Skip "> "
                    sys_write_to_file(filename, message_start + 1);
                } else {
                    printf("Error: Missing '>' in echo command.\n");
                }
            } else {
                printf("Error: Missing closing quote in echo command.\n");
            }
        }
    } else if (strncmp(command, "cat ", 4) == 0) {
        sys_read_file(command + 4);
    } else if (strncmp(command, "rm ", 3) == 0) {
        sys_delete_file(command + 3);
    } else if (strcmp(command, "ls") == 0) {
        sys_list_directory(false); // Don't show hidden files
    } else if (strcmp(command, "ls -a") == 0) {
        sys_list_directory(true); // Show hidden files
    } else if (strncmp(command, "mkdir ", 6) == 0) {
        sys_create_directory(command + 6);
    } else if (strncmp(command, "cd ", 3) == 0) {
        sys_change_directory(command + 3);
    } else if (strncmp(command, "rmdir ", 6) == 0) {
        sys_delete_directory(command + 6);
    } else {
        printf("Unknown command: %s\n", command);
    }
}

