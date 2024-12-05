#include <stdio.h>
#include <stdlib.h>
#include "process.h"

// Process queue
ProcessQueue process_queue = { .head = NULL, .tail = NULL };

// Create a new process
Process* create_process(int pid, int execution_time) {
    Process *new_process = (Process *)malloc(sizeof(Process));
    if (new_process == NULL) {
        perror("Failed to allocate process");
        return NULL;
    }
    new_process->pid = pid;
    new_process->time_remaining = execution_time;
    new_process->state = READY;
    new_process->next = NULL;
    enqueue_process(new_process);
    printf("Process PID=%d created with execution time=%d.\n", pid, execution_time);
    return new_process;
}

// Enqueue a process to the ready queue
void enqueue_process(Process *process) {
    if (process_queue.tail == NULL) {
        process_queue.head = process;
        process_queue.tail = process;
    } else {
        process_queue.tail->next = process;
        process_queue.tail = process;
    }
}

// Dequeue the next ready process
Process* dequeue_process() {
    if (process_queue.head == NULL) {
        return NULL;
    }
    Process *process = process_queue.head;
    process_queue.head = process->next;
    if (process_queue.head == NULL) {
        process_queue.tail = NULL;
    }
    return process;
}

// Get the next ready process
Process* get_next_ready_process() {
    return dequeue_process();
}

// Terminate a process
void terminate_process(int pid) {
    Process *prev = NULL;
    Process *curr = process_queue.head;

    // Find the process in the queue
    while (curr != NULL) {
        if (curr->pid == pid) {
            printf("Terminating process PID=%d.\n", pid);

            // Remove the process from the queue
            if (prev == NULL) {
                // The process is at the head of the queue
                process_queue.head = curr->next;
            } else {
                prev->next = curr->next;
            }

            if (curr == process_queue.tail) {
                // The process is at the tail of the queue
                process_queue.tail = prev;
            }

            // Free the process memory
            free(curr);
            return;
        }
        prev = curr;
        curr = curr->next;
    }

    printf("Process PID=%d not found in the queue.\n", pid);
}