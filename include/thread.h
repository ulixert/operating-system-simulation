#ifndef THREAD_H
#define THREAD_H

#include <stdbool.h>

// Thread states
typedef enum {
    THREAD_READY,
    THREAD_RUNNING,
    THREAD_TERMINATED
} ThreadState;

// Thread structure
typedef struct Thread {
    int tid; // Thread ID
    int time_remaining; // Execution time
    ThreadState state; // Thread state
    void (*function)(void *); // Function pointer for the thread's task
    void *arg; // Arguments for the thread's task
    struct Thread *next; // Pointer to the next thread
} Thread;

typedef struct {
    Thread *head;
    Thread *tail;
} ThreadQueue;

// Thread management functions
void initialize_thread_queue(ThreadQueue *queue);

void create_thread(ThreadQueue *queue, int tid, int time_required, void (*function)(void *), void *arg);

Thread *dequeue_thread(ThreadQueue *queue);

void enqueue_thread(ThreadQueue *queue, Thread *thread);

void execute_thread(ThreadQueue *queue);

// system threads
void io_handler_thread(void *arg);

void logger_thread(void *arg);

void resource_monitor_thread(void *arg);

// read-only threads
void idle_thread(void *arg);

#endif
