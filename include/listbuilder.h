#ifndef HUFFMANOCOOL_LISTBUILDER_H
#define HUFFMANOCOOL_LISTBUILDER_H


typedef struct frequency_linked_list_node
{
    unsigned char byte;
    unsigned long count;
    struct frequency_linked_list* next;
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
ListNode* gen_list_from_buffer(unsigned char * buffer, unsigned long size_buffer);


#endif //HUFFMANOCOOL_LISTBUILDER_H
