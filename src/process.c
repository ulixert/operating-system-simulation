#include <stdio.h>
#include <stdlib.h>
#include "process.h"

ProcessQueue process_queue = {.head = NULL, .tail = NULL};
Process *current_process = NULL;
extern ThreadQueue system_thread_queue;

void initialize_process_queue() {
    process_queue.head = process_queue.tail = NULL;
}

void create_process(int pid, int time_required) {
    Process *new_process = (Process *) malloc(sizeof(Process));
    new_process->pid = pid;
    new_process->time_remaining = time_required;
    new_process->state = READY;
    new_process->next = NULL;

    enqueue_process(new_process);
    printf("Process created: PID=%d, Time Required=%d\n", pid, time_required);
}

Process *dequeue_process() {
    if (process_queue.head == NULL) return NULL;

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

void terminate_process(int pid) {
    if (current_process && current_process->pid == pid) {
        free(current_process);
        current_process = NULL;
        printf("Process PID=%d terminated.\n", pid);
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

            free(curr);
            printf("Process PID=%d terminated.\n", pid);
            return;
        }
        prev = curr;
        curr = curr->next;
    }

    printf("Error: Process PID=%d not found.\n", pid);
}

void list_processes() {
    printf("PID\tState\t\tTime Remaining\n");
    if (current_process) {
        printf("%d\tRUNNING\t\t%d\n", current_process->pid, current_process->time_remaining);
    }

    Process *curr = process_queue.head;
    while (curr) {
        printf("%d\t%s\t\t%d\n", curr->pid,
               curr->state == READY ? "READY" : "TERMINATED",
               curr->time_remaining);
        curr = curr->next;
    }
}

void list_processes_and_threads() {
    printf("Processes:\n");
    list_processes();

    printf("\nSystem Threads:\n");
    Thread *current = system_thread_queue.head;
    while (current != NULL) {
        printf("TID=%d (System Thread)\n", current->tid);
        current = current->next;
    }
}

void schedule_next_process() {
    // If there is a currently running process, move it back to the READY state
    if (current_process != NULL) {
        if (current_process->time_remaining > 0) {
            current_process->state = READY;
            enqueue_process(current_process); // Requeue the process
        } else {
            // Process has completed, free it
            terminate_process(current_process->pid);
        }
    }

    // Dequeue the next process to execute
    current_process = dequeue_process();
    if (current_process != NULL) {
        current_process->state = RUNNING; // Set the state to RUNNING
        printf("Scheduler: Scheduled process PID=%d\n", current_process->pid);
    } else {
        printf("Scheduler: No process to schedule.\n");
    }
}

Process *find_process_by_pid(int pid) {
    // Check the currently running process
    if (current_process != NULL && current_process->pid == pid) {
        return current_process;
    }

    // Search in the process queue
    Process *current = process_queue.head;
    while (current != NULL) {
        if (current->pid == pid) {
            return current;
        }
        current = current->next;
    }

    // Process not found
    return NULL;
}
