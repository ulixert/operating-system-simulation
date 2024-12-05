#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include "system_services.h"

// Create an empty file
bool create_file_service(const char *name) {
    FILE *file = fopen(name, "w");
    if (!file) {
        perror("Failed to create file");
        return false;
    }
    fclose(file);
    return true;
}

// Write data to a file
bool write_to_file_service(const char *name, const char *data) {
    FILE *file = fopen(name, "w");
    if (!file) {
        perror("Failed to open file for writing");
        return false;
    }
    fprintf(file, "%s", data);
    fclose(file);
    return true;
}

// Read data from a file
bool read_file_service(const char *name) {
    FILE *file = fopen(name, "r");
    if (!file) {
        perror("Failed to open file for reading");
        return false;
    }

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file)) {
        printf("%s", buffer);
    }
    fclose(file);
    return true;
}

// Delete a file
bool delete_file_service(const char *name) {
    return (remove(name) == 0);
}

// List directory contents
void list_directory_service() {
    DIR *dir = opendir(".");
    if (!dir) {
        perror("Failed to list directory");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        printf("%s%s\n", entry->d_name, (entry->d_type == DT_DIR) ? "/" : "");
    }
    closedir(dir);
}

// Create a directory
bool create_directory_service(const char *name) {
    return (mkdir(name, 0755) == 0);
}

// Change the current working directory
bool change_directory_service(const char *name) {
    return (chdir(name) == 0);
}

// Delete a directory
bool delete_directory_service(const char *name) {
    return (rmdir(name) == 0);
}