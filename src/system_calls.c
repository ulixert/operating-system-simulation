#include <stdio.h>
#include "system_calls.h"
#include "system_services.h"

// Create a file
bool sys_create_file(const char *name) {
    if (!create_file_service(name)) {
        printf("Error: Failed to create file '%s'.\n", name);
        return false;
    }
    printf("File '%s' created successfully.\n", name);
    return true;
}

// Write to a file
bool sys_write_to_file(const char *name, const char *data) {
    if (!write_to_file_service(name, data)) {
        printf("Error: Failed to write to file '%s'.\n", name);
        return false;
    }
    printf("Data written to '%s'.\n", name);
    return true;
}

// Read a file
bool sys_read_file(const char *name) {
    if (!read_file_service(name)) {
        printf("Error: Failed to read file '%s'.\n", name);
        return false;
    }
    return true;
}

// Delete a file
bool sys_delete_file(const char *name) {
    if (!delete_file_service(name)) {
        printf("Error: Failed to delete file '%s'.\n", name);
        return false;
    }
    printf("File '%s' deleted successfully.\n", name);
    return true;
}

// List directory contents
void sys_list_directory(bool show_all) {
    list_directory_service();
}

// Create a directory
bool sys_create_directory(const char *name) {
    if (!create_directory_service(name)) {
        printf("Error: Failed to create directory '%s'.\n", name);
        return false;
    }
    printf("Directory '%s' created successfully.\n", name);
    return true;
}

// Change the current working directory
bool sys_change_directory(const char *name) {
    if (!change_directory_service(name)) {
        printf("Error: Failed to change directory to '%s'.\n", name);
        return false;
    }
    printf("Changed directory to '%s'.\n", name);
    return true;
}

// Delete a directory
bool sys_delete_directory(const char *name) {
    if (!delete_directory_service(name)) {
        printf("Error: Failed to delete directory '%s'.\n", name);
        return false;
    }
    printf("Directory '%s' deleted successfully.\n", name);
    return true;
}