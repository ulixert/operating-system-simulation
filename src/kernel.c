#include <stdio.h>
#include "kernel.h"

#include <interrupt.h>

#include "process.h"

ThreadQueue system_thread_queue;

void initialize_kernel() {
    initialize_process_queue();
    initialize_thread_queue(&system_thread_queue);
    initialize_interrupts();

    // Create system threads
    static int tid_counter = 1000; // System thread IDs start at 1000
    create_thread(&system_thread_queue, tid_counter++, 0, io_handler_thread, NULL);
    create_thread(&system_thread_queue, tid_counter++, 0, logger_thread, NULL);
    create_thread(&system_thread_queue, tid_counter++, 0, resource_monitor_thread, NULL);

    printf("Kernel initialized with system threads.\n");
}

void handle_interrupt(int interrupt_type) {
    if (interrupt_type == TIMER_INTERRUPT) {
        schedule_next_process();
    } else {
        printf("Kernel: Unknown interrupt.\n");
    }
}
