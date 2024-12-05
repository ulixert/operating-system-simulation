#include <cpu.h>
#include <stdio.h>
#include "include/user.h"
#include "include/kernel.h"
#include "include/interrupt.h"
#include "include/system_calls.h"

int main() {
    initialize_cpu();

    printf("Welcome to the OS Simulation!\n");
    initialize_kernel();
    user_shell();
    return 0;
}