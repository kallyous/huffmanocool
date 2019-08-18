#ifndef HUFFMANOCOOL_ARGDEFS_H
#define HUFFMANOCOOL_ARGDEFS_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "stdoutdebug.h"

unsigned int INPUT_FILE_SIZE = 0;
unsigned int FILE_NAME_SIZE = 0;
char * FILE_NAME_STR;

bool parse_arguments(int argc, char * argv[])
{
    load_debug_setting(argc, argv);

    if (argc < 2) {
        printf("Argumentos insuficientes fornecidos.\n");
        printf("Usar: huffmanocool --file-path caminho_do_arquivo --file-size tamanho_em_bytes\n");
        return false;
    }

    for (int i=0; i < argc; i++)
    {
        dfprint("Parsing '%s'\n", argv[i]);
        if (strcmp(argv[i], "--file-size") == 0) {
            INPUT_FILE_SIZE = atoi(argv[i+1]);
            dfprint("Input file size: %d\n", INPUT_FILE_SIZE);
        }
        if (strcmp(argv[i], "--file-path") == 0) {
            FILE_NAME_SIZE = strlen(argv[i+1]);
            FILE_NAME_STR = (char*)malloc((FILE_NAME_SIZE * sizeof(char))+1);
            strcpy(FILE_NAME_STR, argv[i+1]);
            dfprint("Input file name: '%s'\n", FILE_NAME_STR);
        }
    }

    if (FILE_NAME_SIZE == 0)
    {
        printf("Caminho do arquivo não informado.\n");
        printf("Usar: huffmanocool --file-path caminho_do_arquivo --file-size tamanho_em_bytes\n");
        return false;
    }

    if (INPUT_FILE_SIZE == 0)
    {
        printf("Tamanho do arquivo não informado.\n");
        printf("Usar: huffmanocool --file-path caminho_do_arquivo --file-size tamanho_em_bytes\n");
        return false;
    }

    dfprint("Arguments parsed.\n");

    return true;
}

#endif //HUFFMANOCOOL_ARGDEFS_H
