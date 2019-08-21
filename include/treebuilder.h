#ifndef HUFFMANOCOOL_TREEBUILDER_H
#define HUFFMANOCOOL_TREEBUILDER_H


#include "globals.h"
#include "listbuilder.h"


typedef struct huffman_tree_node
{
    unsigned char byte;
    unsigned int weight;
    struct huffman_tree_node* left;
    struct huffman_tree_node* right;

} TreeNode;


/*  Gera um novo nodo de arvore, desconectado
 *
 */
TreeNode* EmptyTreeNode();


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
TreeNode* gen_huffman_tree_from_frequency_list(ListNode* freq_list);


#endif //HUFFMANOCOOL_TREEBUILDER_H
