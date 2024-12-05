#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "user.h"
#include "process.h"
#include "thread.h"
#include "system_calls.h"

extern char project_root[1024];

void user_shell() {
    char command_line[256];
    char cwd[1024];

    printf("Hello, User! Welcome to the OS Simulation!\n");
    printf("Type 'help' to see the list of available commands.\n");

    while (1) {
        if (getcwd(cwd, sizeof(cwd)) == NULL) {
            perror("Failed to get current working directory");
            break;
        }

        printf("\nuser ");

        if (strcmp(cwd, project_root) == 0) {
            printf("~> ");
        } else if (strncmp(cwd, project_root, strlen(project_root)) == 0) {
            printf("~%s> ", cwd + strlen(project_root));
        } else {
            printf("%s> ", cwd);
        }

        if (fgets(command_line, sizeof(command_line), stdin) == NULL) {
            break; // EOF detected (e.g., Ctrl+D)
        }

        command_line[strcspn(command_line, "\n")] = 0; // Remove trailing newline

        if (strlen(command_line) == 0) {
            continue; // Empty input; prompt again
        }

        if (strcmp(command_line, "exit") == 0)
            break;

        handle_user_command(command_line);
    }
}

void handle_user_command(const char *input_command) {
    char command[256];
    strncpy(command, input_command, sizeof(command));
    command[sizeof(command) - 1] = '\0'; // Ensure null termination

    char *args[16]; // Maximum of 16 arguments
    int arg_count = 0;

    // Tokenize the input command
    char *token = strtok(command, " ");
    while (token != NULL && arg_count < 16) {
        args[arg_count++] = token;
        token = strtok(NULL, " ");
    }

    if (arg_count == 0) {
        // Empty command
        return;
    }

    // Now, args[0] is the command, args[1...] are the arguments
    // Proceed to handle commands

    if (strcmp(args[0], "help") == 0) {
        if (arg_count == 1) {
            // General help
            printf("Available commands:\n");
            printf("For file system:\n");
            printf("  ls                        - List unhidden files and directories\n");
            printf("  ls -a                     - List all files and directories\n");
            printf("  touch <file>              - Create a file\n");
            printf("  rm <file>                 - Delete a file\n");
            printf("  mkdir <dir>               - Create a directory\n");
            printf("  rmdir <dir>               - Remove a directory\n");
            printf("  cd <dir>                  - Change directory (cd or cd ~ to go to home)\n");
            printf("  mv <src> <dest>           - Move a file or directory\n");
            printf("  rename <old> <new>        - Rename a file or directory\n");
            printf("  echo \"message\" > <file>   - Write to a file\n");
            printf("  cat <file>                - Read a file\n");

            printf("\nFor processes and threads:\n");
            printf("  ps                        - List all processes\n");
            printf("  kill <pid>                - Terminate a process\n");
            printf("  run <command> <time>      - Create a process (time in seconds, emit or -1 for infinite)\n");
            printf(
                "  thread <pid> <time>       - Create a thread in a process (time in seconds, omit or -1 for infinite)\n");

            printf("\nOther commands:\n");
            printf("  exit                      - Exit the OS simulation\n");
            printf("  help                      - Display this help message\n");
        } else {
            // Help for specific command
            printf("Specific help not implemented yet.\n");
        }
    } else if (strcmp(args[0], "cd") == 0) {
        if (arg_count == 1 || (arg_count == 2 && strcmp(args[1], "~") == 0)) {
            // Go to project root
            if (chdir(project_root) != 0) {
                perror("Failed to change directory");
            }
        } else if (arg_count == 2) {
            sys_call_change_directory(args[1]);
        } else {
            printf("Usage: cd <dir>\n");
        }
    } else if (strcmp(args[0], "ls") == 0) {
        if (arg_count == 1) {
            sys_call_list_directory(false);
        } else if (arg_count == 2 && strcmp(args[1], "-a") == 0) {
            sys_call_list_directory(true);
        } else {
            printf("Usage: ls or ls -a\n");
        }
    } else if (strcmp(args[0], "rename") == 0) {
        if (arg_count != 3) {
            printf("  rename <old> <new>        - Rename a file or directory\n");
        } else {
            sys_call_rename(args[1], args[2]);
        }
    } else if (strcmp(args[0], "touch") == 0) {
        if (arg_count != 2) {
            printf("Usage: touch <file>\n");
        } else {
            sys_call_create_file(args[1]);
        }
    } else if (strcmp(args[0], "echo") == 0) {
        // Handle echo command
        // For simplicity, let's handle only 'echo "message" > file'
        char *message_start = strchr(input_command, '"');
        if (message_start) {
            char *message_end = strchr(message_start + 1, '"');
            if (message_end) {
                *message_end = '\0';
                char *redirect = strstr(message_end + 1, ">");
                if (redirect) {
                    char *filename = redirect + 1;
                    while (*filename == ' ' || *filename == '>') filename++; // Skip spaces and '>'
                    sys_call_write_to_file(filename, message_start + 1);
                } else {
                    printf("Error: Missing '>' in echo command.\n");
                }
            } else {
                printf("Error: Missing closing quote in echo command.\n");
            }
        } else {
            printf("Usage: echo \"message\" > <file>\n");
        }
    } else if (strcmp(args[0], "cat") == 0) {
        if (arg_count != 2) {
            printf("Usage: cat <file>\n");
        } else {
            sys_call_read_file(args[1]);
        }
    } else if (strcmp(args[0], "rm") == 0) {
        if (arg_count != 2) {
            printf("Usage: rm <file>\n");
        } else {
            sys_call_delete_file(args[1]);
        }
    } else if (strcmp(args[0], "mkdir") == 0) {
        if (arg_count != 2) {
            printf("Usage: mkdir <dir>\n");
        } else {
            sys_call_create_directory(args[1]);
        }
    } else if (strcmp(args[0], "rmdir") == 0) {
        if (arg_count != 2) {
            printf("Usage: rmdir <dir>\n");
        } else {
            sys_call_delete_directory(args[1]);
        }
    } else if (strcmp(args[0], "mv") == 0) {
        if (arg_count != 3) {
            printf("Usage: mv <source> <destination>\n");
        } else {
            sys_call_move(args[1], args[2]);
        }
    } else if (strcmp(args[0], "run") == 0) {
        if (arg_count < 2 || arg_count > 3) {
            printf("Usage: run <command> <time>\n");
        } else {
            const char *name = args[1];
            int time_required = (arg_count == 3) ? atoi(args[2]) : -1;
            if (time_required > 0 || time_required == -1) {
                static int pid_counter = 120; // Start PID from 100 for user processes
                int ppid = 0; // Parent PID is 0 for simplicity
                create_process(pid_counter++, ppid, 1000, time_required, name, true);
            } else {
                printf("Error: Invalid time specified.\n");
            }
        }
    } else if (strcmp(args[0], "ps") == 0) {
        if (arg_count == 1) {
            list_processes_and_threads();
        } else {
            printf("Usage: ps\n");
        }
    } else if (strcmp(args[0], "kill") == 0) {
        if (arg_count != 2) {
            printf("Usage: kill <pid>\n");
        } else {
            int pid = atoi(args[1]);
            if (pid > 0) {
                terminate_process(pid);
            } else {
                printf("Error: Invalid PID specified.\n");
            }
        }
    } else if (strcmp(args[0], "thread") == 0) {
        if (arg_count < 2 || arg_count > 3) {
            printf("Usage: thread <pid> <time>\n");
        } else {
            int pid = atoi(args[1]);
            int time_required = (arg_count == 3) ? atoi(args[2]) : -1;
            Process *process = find_process_by_pid(pid);
            if (process == NULL) {
                printf("Error: Process PID=%d not found.\n", pid);
            } else {
                static int tid_counter = 1050; // Simple TID generator
                char thread_name[256];
                sprintf(thread_name, "%s_thread_%d", process->command, ++process->thread_count);
                create_thread(&process->threads, tid_counter++, time_required, dummy_thread_task, NULL, thread_name,
                              true);
            }
        }
    } else {
        printf("Unknown or incomplete command: %s\n", args[0]);
        // Show help for the command if possible
        if (strcmp(args[0], "rename") == 0) {
            printf("  rename <old> <new>        - Rename a file or directory\n");
        } else if (strcmp(args[0], "cd") == 0) {
            printf("Usage: cd <dir>\n");
        } else if (strcmp(args[0], "ls") == 0) {
            printf("Usage: ls or ls -a\n");
        } else if (strcmp(args[0], "mv") == 0) {
            printf("Usage: mv <source> <destination>\n");
        } else if (strcmp(args[0], "touch") == 0) {
            printf("Usage: touch <file>\n");
        } else if (strcmp(args[0], "echo") == 0) {
            printf("Usage: echo \"message\" > <file>\n");
        } else if (strcmp(args[0], "cat") == 0) {
            printf("Usage: cat <file>\n");
        } else if (strcmp(args[0], "rm") == 0) {
            printf("Usage: rm <file>\n");
        } else if (strcmp(args[0], "mkdir") == 0) {
            printf("Usage: mkdir <dir>\n");
        } else if (strcmp(args[0], "rmdir") == 0) {
            printf("Usage: rmdir <dir>\n");
        } else if (strcmp(args[0], "run") == 0) {
            printf("Usage: run <command> <time>\n");
        } else if (strcmp(args[0], "kill") == 0) {
            printf("Usage: kill <pid>\n");
        } else if (strcmp(args[0], "thread") == 0) {
            printf("Usage: thread <pid> <time>\n");
        } else {
            printf("Type 'help' to see the list of available commands.\n");
        }
    }
}
