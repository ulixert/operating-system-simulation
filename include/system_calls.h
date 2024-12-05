#ifndef SYSTEM_CALLS_H
#define SYSTEM_CALLS_H

#include <stdbool.h>

// File system calls
bool sys_create_file(const char *name);

bool sys_write_to_file(const char *name, const char *data);

bool sys_read_file(const char *name);

bool sys_delete_file(const char *name);

void sys_list_directory(bool show_all);

bool sys_create_directory(const char *name);

bool sys_change_directory(const char *name);

bool sys_delete_directory(const char *name);

bool sys_move(const char *source, const char *destination);

bool sys_rename(const char *old_name, const char *new_name);

#endif
