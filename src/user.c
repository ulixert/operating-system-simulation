#include <stdio.h>
#include <string.h>
#include "user.h"
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
    } else {
        printf("Unknown command: %s\n", command);
    }
}