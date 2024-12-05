#ifndef SYSTEM_SERVICES_H
#define SYSTEM_SERVICES_H

#include <stdbool.h>

bool create_file_service(const char *name);

bool write_to_file_service(const char *name, const char *data);

bool read_file_service(const char *name);

bool delete_file_service(const char *name);

void list_directory_service(bool show_all);

bool create_directory_service(const char *name);

bool change_directory_service(const char *name);

bool delete_directory_service(const char *name);

bool move_service(const char *source, const char *destination);

bool rename_service(const char *old_name, const char *new_name);

#endif
