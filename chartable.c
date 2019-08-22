#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "include/chartable.h"



void build_packing_table(HufNode* node, char* table[], char* code)
{
//    // Caso seja uma folha, armazene em table, no indice de seu respectivo byte, o caminho percorrido.
//    if (!(node->left || node->right)) {
//        strcpy(table[node->byte], code);
//        return; }
//
//    char* next_code = (char*)malloc(sizeof(char) * 9);
//
//    sprintf(next_code, "%s0", code); // Adiciona o 0 para descer aa esquerda.
//    build_packing_table(node->left, table, next_code);
//
//    sprintf(next_code, "%s1", code); // Adiciona o 1 para descer aa direita
//    build_packing_table(node->right, table, next_code);
}
