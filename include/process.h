#ifndef PROCESS_H
#define PROCESS_H
#include <thread.h>

typedef enum {
    READY,
    RUNNING,
    TERMINATED
} ProcessState;

typedef struct Process {
    int pid;
    int time_remaining;
    ProcessState state;
    ThreadQueue threads;
    struct Process *next;
} Process;

typedef struct {
    Process *head;
    Process *tail;
} ProcessQueue;

extern ProcessQueue process_queue;

extern Process *current_process;

void initialize_process_queue();

void create_process(int pid, int time_required);

Process *dequeue_process();

void enqueue_process(Process *process);

void terminate_process(int pid);

void list_processes_and_threads();

void schedule_next_process();

Process *find_process_by_pid(int pid);

#endif
