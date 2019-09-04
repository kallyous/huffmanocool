#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "globals.h"
#include "assist.h"
#include "debug.h"


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


byte* append_byte(byte* buffer, byte one_byte, unsigned long buffer_load, unsigned long buffer_length)
{
    if (buffer_load < buffer_length) buffer[buffer_load] = one_byte;
    return buffer;
}
