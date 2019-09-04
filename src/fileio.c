#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "globals.h"
#include "assist.h"
#include "debug.h"
#include "fileio.h"


byte* load_file_into_buffer(byte * file_path, unsigned long * buffer_length)
{
    FILE * fptr;
    size_t result;
    int temp_length;
    byte * buffer;

    fptr = fopen(file_path , "r");
    if (fptr == NULL) {
        dfprint("Arquivo '%s' não existe.\n", file_path);
        return NULL; }

    // Obtem tamanho do arquivo
    fseek(fptr, 0, SEEK_END);
    temp_length = ftell(fptr);
    rewind(fptr);

    // Alloca memória para conter arquivo
    buffer = (byte*)malloc(sizeof(byte) * temp_length);
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


int write_from_buffer(byte * file_name, byte * buffer, unsigned long buffer_length)
{
    FILE * fptr;

    fptr = fopen(file_name, "w");

    fwrite(buffer, 1, buffer_length, fptr);

    fclose(fptr);

    return 0;
}

