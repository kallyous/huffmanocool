#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <criterion/criterion.h>

#include "include/fileloader.h"

Test(fileloader, load_file_into_buffer__bad_file_path)
{
    unsigned char * buffer;
    unsigned long buffer_length;

    char file_path[128];
    strcpy(file_path, "nothing/to/see/here");

    bool result = load_file_into_buffer(file_path, buffer, &buffer_length);
    cr_assert(!result, "Can't load file with bad path name, return false.");
}


Test(fileloader, load_file_into_buffer__arguments_corrent)
{
    unsigned char * buffer;
    unsigned long buffer_length;

    char file_path[128];
    strcpy(file_path, "assets/girl-heavy-armor.jpg");

    char msg[128];
    snprintf(msg, sizeof(msg), "Load test file at %s", file_path);

    bool result = load_file_into_buffer(file_path, buffer, &buffer_length);

    cr_assert(result, "File should be loaded from 'assets/girl-heavy-armor.jpg'");
}

