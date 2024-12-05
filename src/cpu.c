#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

#include "cpu.h"
#include "process.h"
#include "thread.h"
#include "interrupt.h"
#include "kernel.h"

extern Process *current_process;

// Add a cycle counter
static int cycle_count = 0;
static const int WAKE_UP_INTERVAL = 5; // Every 5 cycles, wake up processes

static pthread_t cpu_thread;
static int running = 1;

void *cpu_execution_loop(void *arg) {
    while (running) {
        // 10 ms sleep to simulate CPU cycle, i. e. 100 cycles per second
        usleep(10000);
        trigger_timer_interrupt();
        handle_interrupts(); // handle_interrupts() calls schedule_next_process() if needed

        if (current_process && current_process->state == RUNNING) {
            execute_cpu_cycle();
        } else {
            execute_idle_thread();
        }

        cycle_count++;
        if (cycle_count % WAKE_UP_INTERVAL == 0) {
            // Every WAKE_UP_INTERVAL cycles, try to wake up processes
            wake_up_processes();
        }
    }
    return NULL;
}

void initialize_cpu() {
    initialize_interrupt_system();
    srand(time(NULL));
    if (pthread_create(&cpu_thread, NULL, cpu_execution_loop, NULL) != 0) {
        perror("Failed to create CPU thread");
    }
}

void execute_cpu_cycle() {
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
        current_process->cpu_time_used++;

        // Randomly change process state to simulate I/O
        int random_value = rand() % 100;
        if (random_value < 10) {
            current_process->state = SLEEPING;
        } else if (random_value < 20) {
            current_process->state = WAITING;
        } else {
            current_process->state = RUNNING;
        }


        // Check if process completed
        if (current_process->time_remaining == 0) {
            current_process->state = TERMINATED;
            terminate_process(current_process->pid);
            current_process = NULL;
        }
    } else {
        execute_idle_thread();
    }
}

void execute_idle_thread() {
    // Simulate idle time
    usleep(50000); // 50ms
}


void finalize_cpu() {
    // Signal the CPU thread to stop
    running = 0;

    // Wait for the CPU thread to finish execution
    pthread_join(cpu_thread, NULL);

    printf("CPU: Finalized.\n");
}
