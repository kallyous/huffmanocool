#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "include/argdefs.h"



int main(int argc, char * argv[])
{
    if (!parse_arguments(argc, argv)) return 255;

    printf("Huffman O'cool XVIII é refresco!\n");

    return 0;
}
