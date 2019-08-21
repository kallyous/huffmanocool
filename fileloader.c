#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "include/globals.h"
#include "include/assist.h"
#include "include/stdoutdebug.h"
#include "include/fileloader.h"



char* load_file_into_buffer(char * file_path, unsigned long * buffer_length)
{
    FILE * fptr;
    size_t result;
    int temp_length;
    char * buffer;

    fptr = fopen(file_path , "r");
    if (fptr == NULL) {
        dfprint("Arquivo '%s' não existe.\n", file_path);
        return NULL;
    }

    // Obtem tamanho do arquivo
    fseek(fptr, 0, SEEK_END);
    temp_length = ftell(fptr);
    rewind(fptr);

    // Alloca memória para conter arquivo
    buffer = (char*)malloc(sizeof(char) * temp_length);
    if (!buffer) {
        dfprint("Erro na allocação de %d bytes.\n", temp_length);
        return NULL;
    }

    // Copia arquivo para o buffer
    result = fread(buffer, 1, temp_length, fptr);
    if (result != temp_length) {
        dfprint("Erro carregando arquivo %s para o buffer.\n", file_path);
        return false;
    }

    // Fecha arquivo
    fclose(fptr);

    *buffer_length = temp_length;

    return buffer;
}



int write_from_buffer(char * file_name, char * buffer, unsigned long buffer_length)
{
    FILE * fptr;

    fptr = fopen(file_name, "w");

    fwrite(buffer, 1, buffer_length, fptr);

    fclose(fptr);

    return 0;
}

