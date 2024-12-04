#include "system_calls.h"
#include "system_services.h"

void sys_call_create_file(const char *name) {
    create_file(name);
}

void sys_call_list_files() {
    list_files();
}