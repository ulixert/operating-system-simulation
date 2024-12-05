#ifndef CPU_H
#define CPU_H

#include "process.h"

// Constants
#define DEFAULT_TIME_SLICE 2

// CPU Structure
typedef struct {
    Process *current_process; // Currently running process
    int time_slice;           // Time slice for scheduling
} CPU;

// Global CPU instance
extern CPU cpu;

// Functions
void initialize_cpu();
void execute_cpu_cycle();
void context_switch(Process *next_process);
void schedule_next_process();

#endif