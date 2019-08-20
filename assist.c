#include <stdlib.h>
#include <stdio.h>
#include "include/globals.h"
#include "include/assist.h"
#include "include/stdoutdebug.h"



char* char_into_binary_str(char c)
{
    char* rep;
    // Pra retornar o ponteiro sem perder o conteudo, usamos alocaçao de memoria ao inves de uma array simples
    rep = (char*)malloc(sizeof(char) * 9);

    // Hack pra suprimir warning no compilador sobre formataçao de string vazia
    sprintf(rep, "%s", "");

    // Capturamos bit a bit e concatenamos 0 ou 1, em ordem, na string
    int bit;
    for (int i=7; i > -1; i--) {
        bit = (c >> i) & 1U;
        sprintf(rep, "%s%d", rep, bit); }

    return rep;
}

char* byte_stream_into_binary_str(char * stream, unsigned long stream_length)
{
    char* binary_str;

    // Pra retornar o ponteiro sem perder o conteudo
    binary_str = (char*)malloc((sizeof(char) * stream_length * 8)+1);

    // Hack pra suprimir warning no compilador sobre formataçao de string vazia
    sprintf(binary_str, "%s", "");

    int bit;
    for (int i=0; i < stream_length; i++){
        dfprint("%s ", char_into_binary_str(stream[i]));
    }

    dfprint("\n");
    return binary_str;
}