#include "kernel.h"
#include "process.h"
#include "thread.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void initialize_kernel() {
    initialize_process_queue();

    static int pid_counter = 1;
    static int tid_counter = 1;

    // Simulate system processes with realistic commands and users
    struct SystemProcessInfo {
        const char *command;
        int uid; // 0 for root, 1000 for user
    } system_processes[] = {
                {"systemd", 0},
                {"kthreadd", 0},
                {"rcu_sched", 0},
                {"sshd", 0},
                {"cron", 0},
                {"apache2", 0},
                {"mysql", 0},
                {"bash", 1000},
                {"vim", 1000},
                {"python", 1000}
            };

    int num_processes = sizeof(system_processes) / sizeof(system_processes[0]);

    for (int i = 0; i < num_processes; i++) {
        create_process(pid_counter++, 0, system_processes[i].uid, -1, system_processes[i].command);
        Process *proc = find_process_by_pid(pid_counter - 1);

        // Each process has multiple threads
        for (int j = 0; j < 2; j++) {
            char thread_name[32];
            snprintf(thread_name, sizeof(thread_name), "%s_thread_%d", proc->command, j + 1);
            create_thread(&proc->threads, tid_counter++, -1, system_process_function, NULL, thread_name);
        }
    }
}

void system_process_function(void *arg) {
    // Simulate work in a loop to keep the thread running
    while (1) {
        // Simulate some work
        usleep(50000); // 50ms
        pthread_testcancel(); // Allow thread to be cancelled
    }
}
