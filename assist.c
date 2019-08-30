#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "include/globals.h"
#include "include/assist.h"
#include "include/stdoutdebug.h"



byte* byte_into_binary_str(byte c)
{
    byte* rep;
    // Pra retornar o ponteiro sem perder o conteudo, usamos alocaçao de memoria ao inves de uma array simples
    rep = (byte*)malloc(sizeof(byte) * 9);

    // Hack pra suprimir warning no compilador sobre formataçao de string vazia
    sprintf(rep, "%s", "");

    // Capturamos bit a bit e concatenamos 0 ou 1, em ordem, na string
    int bit;
    for (int i=7; i > -1; i--) {
        bit = (c >> i) & 1U;
        sprintf(rep, "%s%d", rep, bit); }

    return rep;
}



byte* byte_stream_into_binary_str(byte * stream, unsigned long stream_length)
{
    byte* binary_str;

    // Pra retornar o ponteiro sem perder o conteudo
    binary_str = (byte*)malloc((sizeof(byte) * stream_length * 8)+1);

    // Hack pra suprimir warning no compilador sobre formataçao de string vazia
    sprintf(binary_str, "%s", "");

    int bit;
    for (int i=0; i < stream_length; i++){
        dfprint("%s ", byte_into_binary_str(stream[i]));
    }

    dfprint("\n");
    return binary_str;
}



void dump_table(byte* table[], const byte* file_name)
{
    printf("\n%s\n", file_name);
    FILE* fptr = fopen(file_name, "wb");
    if (fptr)
    {
        byte line[16];
        for (int a=0; a < 16; a++) line[a] = '\0';
        for (int b=0; b < 256; b++)
        {
            sprintf(line, "%d\t%s\n", b, table[b]);
            fputs(line, fptr);
        }
    }
    fclose(fptr);
}



int validate_table(byte* table[])
{
    int i;
    int j;
    int dif;
    for (i=0; i < 256; i++) {
        for (j=i+1; j < 256; j++) {
            dif = strcmp(table[i], table[j]);
            if (!dif) return 0; } }
    return 1;
}