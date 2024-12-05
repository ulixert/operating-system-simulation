#include <stdio.h>
#include <stdlib.h>
#include "thread.h"

#include <unistd.h>

// Initialize the thread queue
void initialize_thread_queue(ThreadQueue *queue) {
    queue->head = NULL;
    queue->tail = NULL;
}

// Create a new thread and add it to the thread queue
void create_thread(ThreadQueue *queue, int tid, int time_required, void (*function)(void *), void *arg) {
    Thread *new_thread = (Thread *) malloc(sizeof(Thread));
    new_thread->tid = tid;
    new_thread->time_remaining = time_required;
    new_thread->state = THREAD_READY;
    new_thread->function = function;
    new_thread->arg = arg;
    new_thread->next = NULL;

    enqueue_thread(queue, new_thread);
    printf("Thread created: TID=%d, Time Required=%d\n", tid, time_required);
}

// Enqueue a thread to the thread queue
void enqueue_thread(ThreadQueue *queue, Thread *thread) {
    thread->next = NULL;
    if (queue->tail == NULL) {
        queue->head = thread;
    } else {
        queue->tail->next = thread;
    }
    queue->tail = thread;
}

// Dequeue a thread from the thread queue
Thread *dequeue_thread(ThreadQueue *queue) {
    if (queue->head == NULL) return NULL;

    Thread *thread = queue->head;
    queue->head = queue->head->next;

    if (queue->head == NULL) {
        queue->tail = NULL;
    }

    return thread;
}

// Execute the next thread in the thread queue
void execute_thread(ThreadQueue *queue) {
    Thread *thread = dequeue_thread(queue);
    if (thread == NULL) {
        printf("No threads to execute.\n");
        return;
    }

    thread->state = THREAD_RUNNING;
    printf("Executing Thread TID=%d\n", thread->tid);

    // Simulate thread execution
    thread->function(thread->arg);
    thread->time_remaining--;

    if (thread->time_remaining > 0) {
        thread->state = THREAD_READY;
        enqueue_thread(queue, thread); // Requeue the thread
    } else {
        thread->state = THREAD_TERMINATED;
        printf("Thread TID=%d terminated.\n", thread->tid);
        free(thread);
    }
}

// System threads
void io_handler_thread(void *arg) {
    while (1) {
        printf("I/O Handler: Managing I/O operations.\n");
        sleep(2); // Simulate periodic checks
    }
}

void logger_thread(void *arg) {
    while (1) {
        printf("Logger: Writing system logs.\n");
        sleep(3); // Simulate periodic logging
    }
}

void resource_monitor_thread(void *arg) {
    while (1) {
        printf("Resource Monitor: Checking system resources.\n");
        sleep(5); // Simulate periodic monitoring
    }
}

// read-only threads
void idle_thread(void *arg) {
    while (1) {
        printf("Idle: CPU is idle.\n");
        sleep(1); // Simulate idle time
    }
}
