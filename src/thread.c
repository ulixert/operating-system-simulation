#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "thread.h"

void initialize_thread_queue(ThreadQueue *queue) {
    queue->head = NULL;
    queue->tail = NULL;
    pthread_mutex_init(&queue->mutex, NULL);
}

void create_thread(ThreadQueue *queue, int tid, int time_required, void (*function)(void *), void *arg,
                   const char *command) {
    Thread *new_thread = (Thread *) malloc(sizeof(Thread));
    if (new_thread == NULL) {
        printf("Error: Failed to allocate memory for thread.\n");
        return;
    }
    new_thread->tid = tid;
    new_thread->time_remaining = time_required;
    new_thread->cpu_time_used = 0;
    new_thread->state = THREAD_READY;
    new_thread->function = function;
    new_thread->arg = arg;
    new_thread->next = NULL;

    snprintf(new_thread->command, sizeof(new_thread->command), "%s", command);

    pthread_mutex_lock(&queue->mutex);
    enqueue_thread(queue, new_thread);
    pthread_mutex_unlock(&queue->mutex);

    // Optionally print thread creation
    // printf("Thread created: TID=%d, Time Required=%d, Command=%s\n", tid, time_required, command);
}

void enqueue_thread(ThreadQueue *queue, Thread *thread) {
    thread->next = NULL;
    if (queue->tail == NULL) {
        queue->head = thread;
    } else {
        queue->tail->next = thread;
    }
    queue->tail = thread;
}

Thread *dequeue_thread(ThreadQueue *queue) {
    if (queue->head == NULL)
        return NULL;

    Thread *thread = queue->head;
    queue->head = queue->head->next;

    if (queue->head == NULL) {
        queue->tail = NULL;
    }

    return thread;
}

void execute_thread(ThreadQueue *queue) {
    pthread_mutex_lock(&queue->mutex);

    Thread *thread = queue->head;

    if (thread == NULL) {
        pthread_mutex_unlock(&queue->mutex);
        return;
    }

    // Remove the thread from the head of the queue
    queue->head = thread->next;
    if (queue->head == NULL) {
        queue->tail = NULL;
    }
    thread->next = NULL;

    thread->state = THREAD_RUNNING;

    pthread_mutex_unlock(&queue->mutex);

    // Simulate thread execution
    usleep(50000); // 50ms time slice

    // Decrement time_remaining only if it's greater than 0
    if (thread->time_remaining > 0) {
        thread->time_remaining--;
    }

    thread->cpu_time_used++;

    pthread_mutex_lock(&queue->mutex);

    // Randomly change thread state
    int random_value = rand() % 100;
    if (random_value < 10) {
        thread->state = THREAD_SLEEPING;
    } else if (random_value < 20) {
        thread->state = THREAD_WAITING;
    } else {
        thread->state = THREAD_RUNNING;
    }

    // Re-enqueue the thread if it hasn't completed execution
    if (thread->time_remaining != 0) {
        // Re-enqueue the thread
        if (queue->tail) {
            queue->tail->next = thread;
            queue->tail = thread;
        } else {
            queue->head = queue->tail = thread;
        }
    } else {
        // Thread has completed execution
        thread->state = THREAD_TERMINATED;
        free(thread);
    }

    pthread_mutex_unlock(&queue->mutex);
}

const char *thread_state_to_string(ThreadState state) {
    switch (state) {
        case THREAD_READY:
            return "Ready";
        case THREAD_RUNNING:
            return "Running";
        case THREAD_SLEEPING:
            return "Sleeping";
        case THREAD_WAITING:
            return "Waiting";
        case THREAD_ZOMBIE:
            return "Zombie";
        case THREAD_TERMINATED:
            return "Terminated";
        default:
            return "Unknown";
    }
}

void execute_idle_thread() {
    // Simulate idle time
    usleep(50000); // 50ms
}

void dummy_thread_task(void *arg) {
    // Simulate work
    usleep(50000); // 50ms
}
