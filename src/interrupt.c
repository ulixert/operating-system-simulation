#include "interrupt.h"
#include <stdio.h>

// Global interrupt flag
int interrupt_flag = 0;

void handle_interrupts() {
    if (interrupt_flag) {
        printf("Handling interrupt.\n");
        // Handle the interrupt (placeholder)
        interrupt_flag = 0; // Reset interrupt flag
    }
}
