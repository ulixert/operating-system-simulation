#ifndef CPU_H
#define CPU_H

#define TIME_SLICE 5 // Define a suitable time slice

void initialize_cpu();

void execute_cpu_cycle();

void execute_idle_thread();

void finalize_cpu();

#endif
