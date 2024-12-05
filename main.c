#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "include/user.h"
#include "include/kernel.h"
#include "include/cpu.h"

char project_root[1024];

void *cpu_execution_loop(void *arg) {
    while (1) {
        execute_cpu_cycle();
        handle_system_events();
        usleep(100000);
    }
    return NULL;
}

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

    initialize_kernel();

    // Create a thread for the CPU execution loop
    pthread_t cpu_thread;
    pthread_create(&cpu_thread, NULL, cpu_execution_loop, NULL);

    // Run the user shell in the main thread
    user_shell();

    // Join the CPU thread before exiting (optional)
    pthread_cancel(cpu_thread); // Cancel the CPU thread
    pthread_join(cpu_thread, NULL);

    return 0;
}
