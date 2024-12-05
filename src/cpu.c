#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "cpu.h"

#include <stdlib.h>

#include "process.h"
#include "thread.h"
#include "interrupt.h"

void execute_cpu_cycle() {
    if (!current_process || current_process->state != RUNNING || current_process->time_slice <= 0) {
        schedule_next_process();
    }

    if (current_process) {
        // Execute the process's threads
        if (current_process->threads.head) {
            execute_thread(&current_process->threads);
        }

        // Decrement time slices
        if (current_process->time_remaining > 0) {
            current_process->time_remaining--;
        }
        current_process->time_slice--;
        current_process->cpu_time_used++; // Increment CPU time used

        // Randomly change process state
        int random_value = rand() % 100;
        if (random_value < 10) {
            current_process->state = SLEEPING;
        } else if (random_value < 20) {
            current_process->state = WAITING;
        } else {
            current_process->state = RUNNING;
        }

        // Check if process has completed
        if (current_process->time_remaining == 0) {
            current_process->state = TERMINATED;
            terminate_process(current_process->pid);
            current_process = NULL;
        }
    } else {
        // No process to execute, CPU is idle
        execute_idle_thread();
    }
}

void schedule_next_process() {
    pthread_mutex_lock(&process_queue.mutex);

    if (current_process && current_process->state != TERMINATED) {
        if (current_process->time_slice <= 0) {
            current_process->state = READY;
            current_process->time_slice = TIME_SLICE;
            enqueue_process(current_process);
            current_process = NULL;
        }
    }

    if (!current_process) {
        current_process = dequeue_process();
        if (current_process) {
            current_process->state = RUNNING;
            current_process->time_slice = TIME_SLICE;
        }
    }

    pthread_mutex_unlock(&process_queue.mutex);
}

void handle_system_events() {
    handle_interrupts();
}
