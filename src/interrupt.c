#include "interrupt.h"
#include <pthread.h>
#include "kernel.h"

static InterruptType pending_interrupt = NO_INTERRUPT;
static pthread_mutex_t interrupt_mutex = PTHREAD_MUTEX_INITIALIZER;

void initialize_interrupt_system() {
    pthread_mutex_init(&interrupt_mutex, NULL);
    pending_interrupt = NO_INTERRUPT;
}

void trigger_timer_interrupt() {
    pthread_mutex_lock(&interrupt_mutex);
    pending_interrupt = TIMER_INTERRUPT;
    pthread_mutex_unlock(&interrupt_mutex);
}

InterruptType get_pending_interrupt() {
    pthread_mutex_lock(&interrupt_mutex);
    InterruptType ret = pending_interrupt;
    pthread_mutex_unlock(&interrupt_mutex);
    return ret;
}

void clear_interrupt(InterruptType type) {
    pthread_mutex_lock(&interrupt_mutex);
    if (pending_interrupt == type) {
        pending_interrupt = NO_INTERRUPT;
    }
    pthread_mutex_unlock(&interrupt_mutex);
}

void handle_interrupts() {
    InterruptType intr = get_pending_interrupt();
    if (intr == TIMER_INTERRUPT) {
        // Clear and handle timer interrupt
        clear_interrupt(TIMER_INTERRUPT);
        // On timer interrupt, schedule next process
        schedule_next_process();
    }
}
