#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>
#include <stdbool.h>

// Thread states
typedef enum {
    THREAD_READY,
    THREAD_RUNNING,
    THREAD_SLEEPING,
    THREAD_WAITING,
    THREAD_ZOMBIE,
    THREAD_TERMINATED
} ThreadState;

// Thread structure
typedef struct Thread {
    int tid;
    int time_remaining;
    int cpu_time_used; // CPU time used
    ThreadState state;
    char command[256];

    void (*function)(void *);

    void *arg;
    struct Thread *next;
} Thread;

// Thread queue
typedef struct {
    Thread *head;
    Thread *tail;
    pthread_mutex_t mutex; // Mutex for thread safety
} ThreadQueue;

// Function prototypes
void initialize_thread_queue(ThreadQueue *queue);

void create_thread(ThreadQueue *queue, int tid, int time_required, void (*function)(void *), void *arg,
                   const char *command, bool will_print);

void enqueue_thread(ThreadQueue *queue, Thread *thread);

Thread *dequeue_thread(ThreadQueue *queue);

void execute_thread(ThreadQueue *queue);

void execute_idle_thread();

void dummy_thread_task(void *arg);

const char *thread_state_to_string(ThreadState state);

#endif // THREAD_H
