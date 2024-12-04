#ifndef SYSTEM_SERVICES_H
#define SYSTEM_SERVICES_H

#define MAX_FILES 100

typedef struct {
    char name[256];
} File;

void create_file(const char *name);
void list_files();

#endif