#ifndef HUFFMANOCOOL_CHARTABLE_H
#define HUFFMANOCOOL_CHARTABLE_H


#include "huffmanstructs.h"



/*  Monta tabela contendo associaçao entre byte -> representaçao na arvore, em binario.
 *
 *  I: Raiz da arvore, tabela de strings, inicializada, do tipo char[256][9], para poder armazenar strings
 *      representando ate 8 bits (8 bytes + '\0'), string inicial contendo "0" para armazenar caminho.
 *
 *  O: Nao ha saida, mas o resultado final fica salvo em char[256][9].
 *
 *  ATENÇAO: table[] deve ser uma array de arrays para strings inicializada e de preferencia vazia.
 */
void build_packing_table(HufNode* node, char* table[], char* code);



#endif //HUFFMANOCOOL_CHARTABLE_H
