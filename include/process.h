#ifndef PROCESS_H
#define PROCESS_H

#include "thread.h"
#include <stdbool.h>
#include <pthread.h>
#include <time.h>

// Process states
typedef enum {
    READY,
    RUNNING,
    SLEEPING,
    TERMINATED
} ProcessState;

// Process structure
typedef struct Process {
    int pid;
    int ppid; // Parent PID
    int uid; // User ID
    int time_remaining;
    int cpu_time_used; // CPU time used
    ProcessState state;
    char command[256];
    ThreadQueue threads;
    struct Process *next;
    int time_slice; // For scheduling
    time_t start_time; // Process start time
} Process;

// Process queue
typedef struct {
    Process *head;
    Process *tail;
    pthread_mutex_t mutex; // Mutex for thread safety
} ProcessQueue;

// Global variables
extern ProcessQueue process_queue;
extern Process *current_process;

// Function prototypes
void initialize_process_queue();

void create_process(int pid, int ppid, int uid, int time_required, const char *command);

void terminate_process(int pid);

void list_processes_and_threads();

void schedule_next_process();

Process *dequeue_process();

void enqueue_process(Process *process);

Process *find_process_by_pid(int pid);

void context_switch(Process *next_process);

const char *process_state_to_string(ProcessState state);

#endif // PROCESS_H
