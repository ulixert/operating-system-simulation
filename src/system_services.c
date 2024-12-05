#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include "system_services.h"
#include <limits.h>
#include <libgen.h>

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

// Move a file or directory
bool move_service(const char *source, const char *destination) {
    struct stat src_stat, dest_stat;

    // Get the source metadata
    if (stat(source, &src_stat) != 0) {
        perror("Failed to access source");
        return false;
    }

    // Check if destination exists
    int dest_exists = (stat(destination, &dest_stat) == 0);

    // Validate: Source is a directory, destination is a file
    if (S_ISDIR(src_stat.st_mode) && dest_exists && !S_ISDIR(dest_stat.st_mode)) {
        printf("Error: Cannot move a directory '%s' into a file '%s'.\n", source, destination);
        return false;
    }

    // Handle moving into a directory
    if (dest_exists && S_ISDIR(dest_stat.st_mode)) {
        // Construct the target path inside the destination directory
        char target_path[PATH_MAX];
        snprintf(target_path, PATH_MAX, "%s/%s", destination, basename((char *)source));

        // Attempt to move the source to the target path
        if (rename(source, target_path) != 0) {
            perror("Failed to move file or directory");
            return false;
        }

        printf("Moved '%s' to '%s'.\n", source, target_path);
        return true;
    }

    // Default move (e.g., rename or move to a new location)
    if (rename(source, destination) != 0) {
        perror("Failed to move file or directory");
        return false;
    }

    printf("Moved '%s' to '%s'.\n", source, destination);
    return true;
}

// Rename a file or directory
bool rename_service(const char *old_name, const char *new_name) {
    // Use the rename system call to rename the file or directory
    if (rename(old_name, new_name) != 0) {
        perror("Failed to rename file or directory");
        return false;
    }

    printf("Renamed '%s' to '%s'.\n", old_name, new_name);
    return true;
}
