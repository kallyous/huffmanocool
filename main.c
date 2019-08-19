#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "include/argdefs.h"
#include "include/fileloader.h"



int main(int argc, char * argv[])
{
    if (!parse_arguments(argc, argv)) return 255;
    printf("Huffman O'cool XVIII é refresco!\n");

    unsigned char * buffer;
    unsigned long buffer_length;
    if (!load_file_into_buffer(FILE_NAME_STR, buffer, &buffer_length)) return 255;



    return 0;
}
