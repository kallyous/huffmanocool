#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "include/globals.h"
#include "include/stdoutdebug.h"
#include "include/argdefs.h"
#include "include/fileloader.h"
#include "include/listbuilder.h"
#include "include/treebuilder.h"



int main(int argc, char * argv[])
{
    if (!parse_arguments(argc, argv)) return 255;
    printf("Huffman O'cool XVIII é refresco!\n");

    unsigned char * buffer;
    unsigned long buffer_length;

    if (!load_file_into_buffer(FILE_NAME_STR, buffer, &buffer_length)) return 255;

    // TODO: ler byte a byte o buffer, contando a quatidade de ocorrências e criando a fila ordenada

    // Conecta listbuilder.c
    gen_list_from_buffer(buffer, buffer_length);

    // Conecta treebuilder.c
    ListNode * n;
    gen_huffman_tree_from_frequency_list(n);

    return 0;
}
