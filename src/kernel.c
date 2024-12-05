#include "kernel.h"

#include <cpu.h>

#include "process.h"
#include "thread.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void initialize_kernel() {
    initialize_process_queue();

    static int system_pid_counter = 1; // Start PID from 1 for system processes
    static int user_pid_counter = 100; // Start PID from 100 for user processes
    static int tid_counter = 1000; // Start TID from 1000 for system threads

    // Seed the random number generator
    srand(time(NULL));

    // Simulate system processes with realistic commands and users
    struct SystemProcessInfo {
        const char *command;
        int uid; // 0 for root, 1000 for user
        int is_infinite; // 1 if process runs indefinitely, 0 otherwise
    } system_processes[] = {
                {"systemd", 0, 1},
                {"kthreadd", 0, 1},
                {"rcu_sched", 0, 1},
                {"sshd", 0, 0},
                {"cron", 0, 0},
                {"apache", 0, 0},
                {"mysql", 0, 1},
                {"bash", 1000, 1},
                {"vim", 1000, 1},
                {"python", 1000, 1}
            };

    int num_processes = sizeof(system_processes) / sizeof(system_processes[0]);

    for (int i = 0; i < num_processes; i++) {
        int time_remaining;
        if (system_processes[i].is_infinite) {
            time_remaining = -1;
        } else {
            // Assign random time between 5 and 300 units
            time_remaining = 5 + rand() % 300; // Random time between 5 and 300
        }

        int pid = system_processes[i].uid == 0 ? system_pid_counter++ : user_pid_counter++;
        create_process(pid, 0, system_processes[i].uid, time_remaining, system_processes[i].command, false);
        Process *proc = find_process_by_pid(pid);

        // Each process has multiple threads
        int num_threads = 1 + rand() % 3; // Random number of threads between 1 and 3
        for (int j = 0; j < num_threads; j++) {
            char thread_name[32];
            snprintf(thread_name, sizeof(thread_name), "%s_thread_%d", proc->command, j + 1);

            int thread_time_remaining;
            if (system_processes[i].is_infinite) {
                thread_time_remaining = -1;
            } else {
                // Assign random time between 5 and 15 units
                thread_time_remaining = 5 + rand() % 11; // Random time between 5 and 15
            }

            proc->thread_count++;
            create_thread(&proc->threads, tid_counter++, thread_time_remaining, system_process_function, NULL,
                          thread_name, false);
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

void schedule_next_process() {
    pthread_mutex_lock(&process_queue.mutex);

    // If current process has run out of time slice or is no longer suitable to run, requeue it
    if (current_process && current_process->state != TERMINATED) {
        if (current_process->time_slice <= 0) {
            current_process->state = READY;
            current_process->time_slice = TIME_SLICE;
            enqueue_process(current_process);
            current_process = NULL;
        }
    }

    // If there's no current process, pick one from the queue
    if (!current_process) {
        current_process = dequeue_process();
        if (current_process) {
            current_process->state = RUNNING;
            current_process->time_slice = TIME_SLICE;
        }
    }

    pthread_mutex_unlock(&process_queue.mutex);
}

void wake_up_processes() {
    pthread_mutex_lock(&process_queue.mutex);

    Process *p = process_queue.head;
    while (p) {
        if (p->state == WAITING || p->state == SLEEPING) {
            // Wake them up by setting them back to READY
            p->state = READY;
        }
        p = p->next;
    }

    if (current_process && (current_process->state == WAITING || current_process->state == SLEEPING)) {
        current_process->state = READY;
        // Re-enqueue the current process so scheduler can pick it again
        enqueue_process(current_process);
        current_process = NULL;
    }

    pthread_mutex_unlock(&process_queue.mutex);
}
