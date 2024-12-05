#include <cpu.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "include/user.h"
#include "include/kernel.h"
#include "include/interrupt.h"
#include "include/system_calls.h"

char project_root[1024];

int main() {
    // Capture the current working directory as the home directory
    if (getcwd(project_root, sizeof(project_root)) == NULL) {
        perror("Failed to get project root");
        return 1;
    }

    // Set the current working directory as the root
    if (chdir(project_root) != 0) {
        perror("Failed to set project root as home directory");
        return 1;
    }

    printf("Welcome to the OS Simulation!\n");
    initialize_kernel();
    user_shell();
    return 0;
}
