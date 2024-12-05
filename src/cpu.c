#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
#include "process.h"

// Define the CPU
CPU cpu = { .current_process = NULL, .time_slice = DEFAULT_TIME_SLICE };

// Initialize the CPU
void initialize_cpu() {
    printf("CPU initialized.\n");
    cpu.current_process = NULL;
    cpu.time_slice = DEFAULT_TIME_SLICE;
}

// Simulate a CPU cycle
void execute_cpu_cycle() {
    if (cpu.current_process == NULL) {
        printf("No process is currently running. CPU idle.\n");
        return;
    }

    // Simulate executing the process
    printf("Executing process: PID=%d, Time Remaining=%d\n",
           cpu.current_process->pid, cpu.current_process->time_remaining);

    // Decrease the time remaining for the current process
    cpu.current_process->time_remaining -= cpu.time_slice;

    // Check if the process is finished
    if (cpu.current_process->time_remaining <= 0) {
        printf("Process PID=%d completed.\n", cpu.current_process->pid);
        terminate_process(cpu.current_process->pid);
        cpu.current_process = NULL;
    } else {
        printf("Process PID=%d preempted. Time Remaining=%d\n",
               cpu.current_process->pid, cpu.current_process->time_remaining);
    }
}

// Perform context switching
void context_switch(Process *next_process) {
    if (next_process == NULL) {
        printf("No process to switch to. CPU idle.\n");
        return;
    }

    if (cpu.current_process != NULL) {
        printf("Context switch: Saving state of PID=%d.\n", cpu.current_process->pid);
    }

    cpu.current_process = next_process;
    printf("Context switch: Loading state of PID=%d.\n", cpu.current_process->pid);
}

// Schedule the next process
void schedule_next_process() {
    Process *next_process = get_next_ready_process();
    context_switch(next_process);
}