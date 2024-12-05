#ifndef SYSTEM_CALLS_H
#define SYSTEM_CALLS_H

#include <stdbool.h>

// File system calls
bool sys_call_create_file(const char *name);

bool sys_call_write_to_file(const char *name, const char *data);

bool sys_call_read_file(const char *name);

bool sys_call_delete_file(const char *name);

void sys_call_list_directory(bool show_all);

bool sys_call_create_directory(const char *name);

bool sys_call_change_directory(const char *name);

bool sys_call_delete_directory(const char *name);

bool sys_call_move(const char *source, const char *destination);

bool sys_call_rename(const char *old_name, const char *new_name);

#endif
