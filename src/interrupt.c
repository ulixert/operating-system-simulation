#include <stdio.h>
#include "interrupt.h"
#include "kernel.h"

void initialize_interrupts() {
    printf("Interrupt system initialized.\n");
}

void timer_interrupt() {
    printf("Interrupt: Timer interrupt.\n");
    handle_interrupt(TIMER_INTERRUPT);
}
