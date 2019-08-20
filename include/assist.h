#ifndef HUFFMANOCOOL_ASSIST_H
#define HUFFMANOCOOL_ASSIST_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char* char_into_binary_str(unsigned char c)
{
    unsigned char* rep;
    // Pra retornar o ponteiro sem perder o conteudo, usamos alocaçao de memoria ao inves de uma array simples
    rep = (unsigned char*)malloc(sizeof(unsigned char) * 9);

    // Hack pra suprimir warning no compilador sobre formataçao de string vazia
    sprintf(rep, "%s", "");

    // Capturamos bit a bit e concatenamos 0 ou 1, em ordem, na string
    int bit;
    for (int i=7; i > -1; i--) {
        bit = (c >> i) & 1U;
        sprintf(rep, "%s%d", rep, bit); }

    return rep;
}


#endif //HUFFMANOCOOL_ASSIST_H
