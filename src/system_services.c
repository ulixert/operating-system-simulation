#include <stdio.h>
#include <string.h>
#include "system_services.h"

File files[MAX_FILES];
int file_count = 0;

void create_file(const char *name) {
    if (file_count >= MAX_FILES) {
        printf("File limit reached.\n");
        return;
    }
    strcpy(files[file_count].name, name);
    file_count++;
    printf("File '%s' created.\n", name);
}

void list_files() {
    printf("Files:\n");
    for (int i = 0; i < file_count; i++) {
        printf("  %s\n", files[i].name);
    }
}