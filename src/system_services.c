#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include "system_services.h"
#include <sys/syslimits.h>

extern char project_root[1024];

// Create an empty file
bool create_file_service(const char *name) {
    // Check if the file exists
    struct stat statbuf;
    if (stat(name, &statbuf) == 0) {
        printf("Error: File '%s' already exists.\n", name);
        return false;
    }

    // Create the file
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

    printf("\n");
    fclose(file);
    return true;
}

// Delete a file
bool delete_file_service(const char *name) {
    return (remove(name) == 0);
}

// List directory contents
void list_directory_service(bool show_all) {
    DIR *dir = opendir(".");
    if (!dir) {
        perror("Failed to list directory");
        return;
    }

    struct dirent *entry;
    int count = 0;

    while ((entry = readdir(dir)) != NULL) {
        // Skip hidden files unless `show_all` is true
        if (!show_all && entry->d_name[0] == '.') {
            continue;
        }

        // Check if the entry is a directory
        struct stat statbuf;
        if (stat(entry->d_name, &statbuf) == 0 && S_ISDIR(statbuf.st_mode)) {
            printf("%-20s", strcat(entry->d_name, "/")); // Add `/` to directories
        } else {
            printf("%-20s", entry->d_name);
        }

        count++;

        if (count % 4 == 0) {
            // Newline after every 4 items
            printf("\n");
        }
    }

    if (count % 4 != 0) {
        printf("\n"); // Final newline if items don't align perfectly
    }

    closedir(dir);
}

// Create a directory
bool create_directory_service(const char *name) {
    return (mkdir(name, 0755) == 0);
}

// Change the current working directory
bool change_directory_service(const char *name) {
    char resolved_target[PATH_MAX]; // Buffer to store the resolved target path
    char resolved_root[PATH_MAX]; // Buffer to store the resolved project root path

    // Resolve the project root to ensure it's absolute
    if (!realpath(project_root, resolved_root)) {
        perror("Failed to resolve project root");
        return false;
    }

    // Resolve the target directory to an absolute path
    if (!realpath(name, resolved_target)) {
        perror("Failed to resolve directory");
        return false;
    }

    // Check if the resolved target path is within the project root
    if (strncmp(resolved_target, resolved_root, strlen(resolved_root)) != 0) {
        printf("Error: Access to '%s' is restricted. Cannot navigate outside the project root.\n", name);
        return false;
    }

    // Change to the directory
    if (chdir(resolved_target) != 0) {
        perror("Failed to change directory");
        return false;
    }

    return true;
}

// Delete a directory
bool delete_directory_service(const char *name) {
    return (rmdir(name) == 0);
}
