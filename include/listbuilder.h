#ifndef HUFFMANOCOOL_LISTBUILDER_H
#define HUFFMANOCOOL_LISTBUILDER_H


typedef struct frequency_linked_list_node
{
    unsigned char byte;
    unsigned long count;
    struct frequency_linked_list * next;
} ListNode;


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
ListNode * gen_list_from_buffer(char * buffer, unsigned long size_buffer);


/*  Gera nodo vazio
 *
 *  O: Ponteiro para o novo nodo vazio.
 */
ListNode * EmptyListNode();


/*  Gera nodo com valores definidos.
 *
 *  I: Byte e sua frequencia.
 *
 *  O: Ponteiro para o novo nodo com valores definidos.
 *
 *  Obs: O valor de ListNode.next continua NULL.
 */
ListNode * FillListNode(char value, unsigned long frequency);


/*  Conecta ListNode's de uma array e retorna a cabeça da lista linkada.
 *
 *  I: Ponteiro para o primeiro elemento de uma array de ponteiros para ListNode,
 *      e o tamanho da array.
 *
 *  O: Cabeça da lista encadeada contendo somente os elementos com ao menos uma
 *      ocorrencia.
 */
ListNode* link_ordered_array(ListNode * arr[], int length);


#endif //HUFFMANOCOOL_LISTBUILDER_H
