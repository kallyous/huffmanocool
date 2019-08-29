#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "include/chartable.h"



char** prepare_packing_table()
{
    int byte_table_length = 256;
    char** table = (char**)malloc(sizeof(char*) * 256);

    // Limpa todas as strings
    for (int i=0; i < byte_table_length; i++) {
        table[i] = (char*)malloc(sizeof(char)*9);
        strcpy(table[i], ""); }

    return table;
}



void build_packing_table(HufNode* node, char* table[], char* code)
{
    // Caso seja uma folha, armazene em table, no indice de seu respectivo byte, o caminho percorrido.
    if (!(node->left || node->right)) {
        strcpy(table[node->one_byte], code);
        return; }

    char next_code[9]; // String com 8 bytes pros 0/1's e o ultimo para '\0'
    for (int i=0; i < 9; i++) next_code[i] = '\0'; // Limpa string

    sprintf(next_code, "%s0", code); // Adiciona o 0 para descer aa esquerda
    build_packing_table(node->left, table, next_code);

    sprintf(next_code, "%s1", code); // Adiciona o 1 para descer aa direita
    build_packing_table(node->right, table, next_code);
}
