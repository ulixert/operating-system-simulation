#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "process.h"

#include <cpu.h>

ProcessQueue process_queue = {.head = NULL, .tail = NULL, .mutex = PTHREAD_MUTEX_INITIALIZER};
Process *current_process = NULL;

void initialize_process_queue() {
    process_queue.head = process_queue.tail = NULL;
    pthread_mutex_init(&process_queue.mutex, NULL);
}

void create_process(int pid, int ppid, int uid, int time_required, const char *command) {
    Process *new_process = (Process *) malloc(sizeof(Process));
    if (new_process == NULL) {
        printf("Error: Failed to allocate memory for process.\n");
        return;
    }
    new_process->pid = pid;
    new_process->ppid = ppid;
    new_process->uid = uid;
    new_process->time_remaining = time_required;
    new_process->cpu_time_used = 0;
    new_process->state = READY;
    new_process->next = NULL;
    new_process->time_slice = TIME_SLICE;
    new_process->start_time = time(NULL);

    // Initialize thread queue
    initialize_thread_queue(&new_process->threads);

    // Set the command
    snprintf(new_process->command, sizeof(new_process->command), "%s", command);

    pthread_mutex_lock(&process_queue.mutex);
    enqueue_process(new_process);
    pthread_mutex_unlock(&process_queue.mutex);

    // Optionally print process creation
    // printf("Process created: PID=%d, PPID=%d, CMD=%s\n", pid, ppid, command);
}

void terminate_process(int pid) {
    pthread_mutex_lock(&process_queue.mutex);

    if (current_process && current_process->pid == pid) {
        // Free threads
        Thread *thread = current_process->threads.head;
        while (thread) {
            Thread *temp = thread;
            thread = thread->next;
            free(temp);
        }
        free(current_process);
        current_process = NULL;
        printf("Process PID=%d terminated.\n", pid);
        pthread_mutex_unlock(&process_queue.mutex);
        return;
    }

    Process *prev = NULL, *curr = process_queue.head;
    while (curr != NULL) {
        if (curr->pid == pid) {
            if (prev == NULL) {
                process_queue.head = curr->next;
            } else {
                prev->next = curr->next;
            }

            if (process_queue.tail == curr) {
                process_queue.tail = prev;
            }

            // Free threads
            Thread *thread = curr->threads.head;
            while (thread) {
                Thread *temp = thread;
                thread = thread->next;
                free(temp);
            }
            free(curr);
            printf("Process PID=%d terminated.\n", pid);
            pthread_mutex_unlock(&process_queue.mutex);
            return;
        }
        prev = curr;
        curr = curr->next;
    }

    pthread_mutex_unlock(&process_queue.mutex);
    printf("Error: Process PID=%d not found.\n", pid);
}

Process *dequeue_process() {
    if (process_queue.head == NULL)
        return NULL;

    Process *process = process_queue.head;
    process_queue.head = process_queue.head->next;

    if (process_queue.head == NULL) {
        process_queue.tail = NULL;
    }

    return process;
}

void enqueue_process(Process *process) {
    process->next = NULL;
    if (process_queue.tail == NULL) {
        process_queue.head = process;
    } else {
        process_queue.tail->next = process;
    }
    process_queue.tail = process;
}

Process *find_process_by_pid(int pid) {
    pthread_mutex_lock(&process_queue.mutex);

    if (current_process && current_process->pid == pid) {
        pthread_mutex_unlock(&process_queue.mutex);
        return current_process;
    }

    Process *current = process_queue.head;
    while (current != NULL) {
        if (current->pid == pid) {
            pthread_mutex_unlock(&process_queue.mutex);
            return current;
        }
        current = current->next;
    }

    pthread_mutex_unlock(&process_queue.mutex);
    return NULL;
}

const char *process_state_to_string(ProcessState state) {
    switch (state) {
        case READY:
            return "Ready";
        case RUNNING:
            return "Running";
        case SLEEPING:
            return "Sleeping";
        case TERMINATED:
            return "Terminated";
        default:
            return "Unknown";
    }
}

void format_cpu_time(int cpu_time, char *buffer, size_t size) {
    int minutes = cpu_time / 60;
    int seconds = cpu_time % 60;
    snprintf(buffer, size, "%02d:%02d", minutes, seconds);
}

void print_process_info(Process *proc) {
    char cpu_time_buf[16];
    format_cpu_time(proc->cpu_time_used, cpu_time_buf, sizeof(cpu_time_buf));

    printf("%-6d %-6s %-10s %-8s %-20s\n",
           proc->pid,
           "Process",
           process_state_to_string(proc->state),
           cpu_time_buf,
           proc->command);

    Thread *thread = proc->threads.head;
    while (thread) {
        format_cpu_time(thread->cpu_time_used, cpu_time_buf, sizeof(cpu_time_buf));

        printf("  %-6d %-6s %-10s %-8s %-20s\n",
               thread->tid,
               "Thread",
               thread_state_to_string(thread->state),
               cpu_time_buf,
               thread->command);
        thread = thread->next;
    }
}

void list_processes_and_threads() {
    pthread_mutex_lock(&process_queue.mutex);

    // Print header
    printf("%-6s %-6s %-10s %-8s %-20s\n",
           "ID", "Type", "State", "CPU Time", "Command");
    printf("--------------------------------------------------------------\n");

    // Display the current process
    if (current_process) {
        print_process_info(current_process);
    }

    // Display queued processes
    Process *process = process_queue.head;
    while (process) {
        print_process_info(process);
        process = process->next;
    }

    pthread_mutex_unlock(&process_queue.mutex);
}
