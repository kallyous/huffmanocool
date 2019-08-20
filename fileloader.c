#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "include/globals.h"
#include "include/stdoutdebug.h"
#include "include/fileloader.h"

bool load_file_into_buffer(char * file_path, unsigned char * buffer, unsigned long * buffer_length)
{
    FILE * fptr;
    size_t result;

    fptr = fopen(file_path , "rb" );
    if (fptr == NULL) {
        dfprint("Arquivo '%s' não existe.\n", file_path);
        return false;
    }

    // Obtem tamanho do arquivo
    fseek(fptr, 0, SEEK_END);
    *buffer_length = ftell(fptr);
    rewind(fptr);

    // Alloca memória para conter arquivo
    buffer = (char*)malloc(sizeof(char) * (*buffer_length));
    if (buffer == NULL) {
        dfprint("Erro na allocação de %lu bytes.\n", *buffer_length);
        return false;
    }

    // Copia arquivo para o buffer
    result = fread(buffer, 1, *buffer_length, fptr);
    if (result != *buffer_length) {
        dfprint("Erro carregando arquivo %s para o buffer.\n", file_path);
        return false;
    }

    // Liberar memoria e termina
    fclose(fptr);
    //free(buffer);

    return true;
}