#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "include/globals.h"
#include "include/stdoutdebug.h"
#include "include/argdefs.h"

extern unsigned int FILE_NAME_SIZE;
extern char * FILE_NAME_STR;

bool parse_arguments(int argc, char * argv[])
{
    FILE_NAME_SIZE = 0;

    load_debug_setting(argc, argv);

    if (argc < 2) {
        printf("Argumentos insuficientes fornecidos\n");
        printf("Usar: huffmanocool --file-path caminho_do_arquivo\n");
        return false;
    }

    for (int i=0; i < argc; i++)
    {
        dfprint("Parsing '%s'\n", argv[i]);
        if (strcmp(argv[i], "--file-path") == 0) {
            if (i == argc-1) {
                printf("Faltando caminho do arquivo após argumento --file-path\n");
                return false;
            }
            FILE_NAME_SIZE = strlen(argv[i+1]);
            FILE_NAME_STR = (char*)malloc((FILE_NAME_SIZE * sizeof(char))+1);
            strcpy(FILE_NAME_STR, argv[i+1]);
            dfprint("Input file name: '%s'\n", FILE_NAME_STR);
        }
    }

    if (FILE_NAME_SIZE == 0)
    {
        printf("Caminho do arquivo não informado.\n");
        printf("Usar: huffmanocool --file-path caminho_do_arquivo\n");
        return false;
    }

    dfprint("Arguments parsed.\n");

    return true;
}
