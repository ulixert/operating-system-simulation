#ifndef INTERRUPT_H
#define INTERRUPT_H

typedef enum {
    NO_INTERRUPT = 0,
    TIMER_INTERRUPT
} InterruptType;

void initialize_interrupt_system();

void trigger_timer_interrupt();

InterruptType get_pending_interrupt();

void clear_interrupt(InterruptType type);

void handle_interrupts();

#endif
