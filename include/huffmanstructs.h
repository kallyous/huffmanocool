#ifndef HUFFMANOCOOL_HUFFMANSTRUCTS_H
#define HUFFMANOCOOL_HUFFMANSTRUCTS_H


#include "globals.h"



typedef struct HufNode
{
    byte value;
    unsigned long count;
    struct HufNode* next;
    struct HufNode* left;
    struct HufNode* right;
} HufNode;



/*  Gera nodo vazio
 *
 *  O: Ponteiro para o novo nodo vazio.
 */
HufNode * EmptyHufNode();



/*  Gera nodo com valores definidos.
 *
 *  I: one_byte e sua frequencia.
 *
 *  O: Ponteiro para o novo nodo com valores definidos.
 *
 *  Obs: O valor de HufNode.next continua NULL.
 */
HufNode * FillHufNode(byte value, unsigned long frequency);



/*  Gera lista encadeada a partir do buffer de bytes recebido.
 *
 *  I: Recebe ponteiro para o buffer contendo a copia dos bytes do arquivo a ser compactado
 *      e o tamanho do buffer.
 *
 *  O: Retorna ponteiro para o primeiro item duma lista encadeada, ordenada crescentemente,
 *      com as frequencias dos bytes do buffer, ou NULL se ocorrer algum erro.
 *
 *  Obs: o buffer permanece inalterado.
 */
HufNode * gen_list_from_buffer(byte * buffer, unsigned long size_buffer);



/*  Conecta HufNode's de uma array e retorna a cabeça da lista linkada.
 *
 *  I: Ponteiro para o primeiro elemento de uma array de ponteiros para HufNode,
 *      e o tamanho da array.
 *
 *  O: Cabeça da lista encadeada contendo somente os elementos com ao menos uma
 *      ocorrencia.
 */
HufNode* link_ordered_array(HufNode * arr[], int length);



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



/*  Gera string no formato pre-ordem a partir de uma arvore.
 *
 *  I: Nodo raiz de uma arvore huffman e buffer a armazenar string sendo construida.
 *
 *  Obs: Caracteres * sao escapados com um '\' antes do '*'. Usa recursao.
 *
 *  ATENÇAO: Esta funçao precisa de testes com todos os 256 valores possiveis
 *          de bytes, para ter certeza qu nao vai bugar com bytes que
 *          representam valores bugados. Nao confiamos em strings de byte.
 */
void build_tree_preorder_array(HufNode* node, byte* buffer, unsigned int * buffer_load, unsigned int buffer_length);



HufNode* rebuild_tree_from_byte_array(const byte* byte_array, unsigned int* curr_index, unsigned int tree_arr_length);



#endif //HUFFMANOCOOL_HUFFMANSTRUCTS_H
