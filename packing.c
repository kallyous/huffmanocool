#include <string.h>

#include "include/packing.h"



unsigned long compress_byte_stream(char* stream, char* buffer, unsigned long stream_length, char* table[], char* garbage_length)
{
    unsigned long i;
    unsigned long final_length = 0;

    char code[9];

    for (i=0; i < stream_length; i++)
    {
        /*  O valor do byte lido de stream[i], quando interpretado como inteiro, eh o indice em table onde esta
         *  a string com a representaçao binaria do byte compactado. Entao apenas acessamos o indice e copiamos
         *  o conteudo (uma string de 0/1's) para dentro de code, e o analisamos caracter a caracter. */
        strcpy(code, table[stream[i]]);
        for (int j=0; j < strlen(code) - 1; j++) { // O -1 eh pra ignorar-mos o '\0' no final da string.
            // TODO: monta mask para definir o valor do bit desejado no byte atual de buffer
            // TODO: verifica na string se o bit sera 0 ou 1
            // TODO: aplica mask e define o bit desejado
            // TODO: atualiza qual byte de buffer esta sendo escrito (sim, podemos passar para outro byte em buffer aind aqui)
            // TODO: atualiza a mask para o proximo bit a ser definido
        }
    }

    // TODO: conta bits de lixo no ultimo byte, se houver, e define garbage_length

    // Terminado. Retorne a quantidade de bytes em uso por buffer
    return final_length;
}