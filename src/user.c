#include <stdio.h>
#include <string.h>
#include "user.h"

#include <cpu.h>
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
        // Create a new process with a specified execution time
        char *time_str = (char *) (command + 4);
        int time_required = atoi(time_str);
        if (time_required > 0) {
            static int pid_counter = 1; // Simple PID generator
            create_process(pid_counter++, time_required);
        } else {
            printf("Error: Invalid time specified.\n");
        }
    } else if (strcmp(command, "ps") == 0) {
        // List all processes
        list_processes_and_threads();
    } else if (strncmp(command, "kill ", 5) == 0) {
        // Terminate a process by PID
        int pid = atoi(command + 5);
        if (pid > 0) {
            terminate_process(pid);
        } else {
            printf("Error: Invalid PID specified.\n");
        }
    } else if (strcmp(command, "step") == 0) {
        // Step the CPU by one cycle
        execute_cpu_cycle();
    } else if (strcmp(command, "exit") == 0) {
        // Exit the simulation
        printf("Exiting OS simulation.\n");
        exit(0);
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
    } else if (strncmp(command, "mv ", 3) == 0) {
        char *args = strchr(command + 3, ' ');
        if (args) {
            *args = '\0';
            const char *source = command + 3;
            const char *destination = args + 1;
            sys_move(source, destination);
        } else {
            printf("Error: Invalid mv syntax. Use 'mv <source> <destination>'.\n");
        }
    } else if (strncmp(command, "rename ", 7) == 0) {
        char *args = strchr(command + 7, ' ');
        if (args) {
            *args = '\0';
            const char *old_name = command + 7;
            const char *new_name = args + 1;
            sys_rename(old_name, new_name);
        } else {
            printf("Error: Invalid rename syntax. Use 'rename <old_name> <new_name>'.\n");
        }
    } else if (strncmp(command, "thread ", 7) == 0) {
        int pid, time_required;
        sscanf(command + 7, "%d %d", &pid, &time_required);

        Process *process = find_process_by_pid(pid);
        if (process == NULL) {
            printf("Error: Process PID=%d not found.\n", pid);
        } else {
            static int tid_counter = 1; // Simple TID generator
            // create_thread(&process->threads, tid_counter++, time_required, dummy_thread_task, NULL);
        }
    } else {
        printf("Unknown command: %s\n", command);
    }
}

