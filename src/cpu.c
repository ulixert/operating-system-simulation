#include <stdio.h>
#include "cpu.h"
#include "interrupt.h"
#include "process.h"

#define TIME_SLICE 3 // Cycles per process

void execute_cpu_cycle() {
    static int cycles = 0;

    if (current_process != NULL) {
        printf("CPU: Executing PID=%d\n", current_process->pid);
        current_process->time_remaining--;

        cycles++;
        if (current_process->time_remaining <= 0) {
            printf("CPU: Process PID=%d completed.\n", current_process->pid);
            terminate_process(current_process->pid);
            cycles = 0;
        } else if (cycles >= TIME_SLICE) {
            printf("CPU: Time slice expired for PID=%d. Triggering timer interrupt.\n", current_process->pid);
            timer_interrupt(); // Trigger timer interrupt
            cycles = 0;
        }
    } else {
        printf("CPU: Idle.\n");
    }
}
