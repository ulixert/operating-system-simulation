#include <stdio.h>
#include "kernel.h"
#include "interrupt.h"
#include "cpu.h"

void initialize_kernel() {
    printf("Kernel initialized.\n");
    initialize_interrupts();
    initialize_cpu();
}