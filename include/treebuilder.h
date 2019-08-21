#ifndef HUFFMANOCOOL_TREEBUILDER_H
#define HUFFMANOCOOL_TREEBUILDER_H


#include "globals.h"
#include "listbuilder.h"


/*  Gera Arvore de Huffman a partir de lista encadeada com frequencia dos
 *  bytes em ordem crescente.
 *
 *  I: Ponteiro para primeiro nodo de uma lista encadeada ordenada crescentemente
 *      contendo em seus nodos os bytes e suas frequencias de ocorrencia no
 *      arquivo sendo compactado.
 *
 *  O: Ponteiro para o nodo raiz da arvore construida, out NULL em caso de erros.
 *
 *  Obs: Nao desaloca/libera memoria da lista encadeada recebida. Isso deve ser
 *          feito noutro lugar.
 */
HufNode* build_huffman_tree(HufNode* freq_list);


#endif //HUFFMANOCOOL_TREEBUILDER_H
