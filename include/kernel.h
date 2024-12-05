#ifndef KERNEL_H
#define KERNEL_H

void initialize_kernel();

void system_process_function(void *arg);

void schedule_next_process();

void wake_up_processes();

#endif
