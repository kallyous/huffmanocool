#ifndef HUFFMANOCOOL_byteTABLE_H
#define HUFFMANOCOOL_byteTABLE_H


#include "globals.h"
#include "huffmanstructs.h"



/*  Monta tabela contendo associaçao entre one_byte -> representaçao na arvore, em binario.
 *
 *  I: Raiz da arvore, tabela de strings, inicializada, do tipo byte[256][9], para poder armazenar strings
 *      representando ate 8 bits (8 bytes + '\0'), string inicial contendo "0" para armazenar caminho.
 *
 *  O: Nao ha saida, mas o resultado final fica salvo em byte[256][9].
 *
 *  ATENÇAO: table[] deve ser uma array de arrays para strings inicializada e de preferencia vazia.
 */
void build_packing_table(HufNode* node, byte* table[], byte* code);



byte** prepare_packing_table();



#endif //HUFFMANOCOOL_byteTABLE_H
