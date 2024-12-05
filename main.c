#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "include/user.h"
#include "include/kernel.h"
#include "include/cpu.h"

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

    // Initialize the kernel, which sets up initial processes and states
    initialize_kernel();

    // Initialize the CPU, which sets up interrupts, creates the CPU thread,
    // and starts the continuous execution loop that triggers interrupts
    // and schedules processes.
    initialize_cpu();

    // Run the user shell in the main thread.
    // The CPU execution loop and interrupt handling run in a separate thread managed by initialize_cpu().
    user_shell();

    // finalize_cpu(); // TODO: Implement finalize_cpu() to stop the CPU thread gracefully

    return 0;
}
