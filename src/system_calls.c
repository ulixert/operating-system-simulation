#include <stdio.h>
#include "system_calls.h"
#include "system_services.h"

// Create a file
bool sys_call_create_file(const char *name) {
    if (!create_file_service(name)) {
        printf("Error: Failed to create file '%s'.\n", name);
        return false;
    }

    printf("File '%s' created successfully.\n", name);
    return true;
}

// Write to a file
bool sys_call_write_to_file(const char *name, const char *data) {
    if (!write_to_file_service(name, data)) {
        printf("Error: Failed to write to file '%s'.\n", name);
        return false;
    }

    printf("Data written to '%s'.\n", name);
    return true;
}

// Read a file
bool sys_call_read_file(const char *name) {
    if (!read_file_service(name)) {
        printf("Error: Failed to read file '%s'.\n", name);
        return false;
    }

    return true;
}

// Delete a file
bool sys_call_delete_file(const char *name) {
    if (!delete_file_service(name)) {
        printf("Error: Failed to delete file '%s'.\n", name);
        return false;
    }

    printf("File '%s' deleted successfully.\n", name);
    return true;
}

// List directory contents
void sys_call_list_directory(bool show_all) {
    list_directory_service(show_all);
}

// Create a directory
bool sys_call_create_directory(const char *name) {
    if (!create_directory_service(name)) {
        printf("Error: Failed to create directory '%s'.\n", name);
        return false;
    }

    printf("Directory '%s' created successfully.\n", name);
    return true;
}

// Change the current working directory
bool sys_call_change_directory(const char *name) {
    if (!change_directory_service(name)) {
        return false;
    }

    return true;
}

// Delete a directory
bool sys_call_delete_directory(const char *name) {
    if (!delete_directory_service(name)) {
        printf("Error: Failed to delete directory '%s'.\n", name);
        return false;
    }

    printf("Directory '%s' deleted successfully.\n", name);
    return true;
}

// Move a file or directory
bool sys_call_move(const char *source, const char *destination) {
    if (!move_service(source, destination)) {
        return false;
    }
    return true;
}

// Rename a file or directory
bool sys_call_rename(const char *old_name, const char *new_name) {
    if (!rename_service(old_name, new_name)) {
        return false;
    }
    return true;
}
