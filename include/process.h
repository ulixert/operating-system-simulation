#ifndef PROCESS_H
#define PROCESS_H

// Process states
typedef enum {
    READY,
    RUNNING,
    BLOCKED,
    TERMINATED
} ProcessState;

// Process structure
typedef struct Process {
    int pid; // Process ID
    int time_remaining; // Remaining execution time
    ProcessState state; // Current state
    struct Process *next; // Next process in the queue
} Process;

// Process queue
typedef struct {
    Process *head;
    Process *tail;
} ProcessQueue;

// Global process queue
extern ProcessQueue process_queue;

// Functions
Process *create_process(int pid, int execution_time);

void enqueue_process(Process *process);

Process *dequeue_process();

Process *get_next_ready_process();

void terminate_process(int pid);

#endif
