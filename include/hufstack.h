#ifndef HUFFMANOCOOL_HUFSTACK_H
#define HUFFMANOCOOL_HUFSTACK_H


#include "huffmanstructs.h"



typedef struct HufStack
{
    unsigned int length;
    HufNode* head;
    HufNode* tail;
} HufStack;



/*  Gera pilha de HufNode vazia.
 *
 */
HufStack* EmptyHufStack();



/*  Empurra nova cabeça (HufNode*) na pilha.
 *
 *  I: Pilha a receber nova cabeça e nodo a se tornar nova cabeça.
 *
 *  O: Novo tamanho da pilha.
 *
 *  Obs: Incrementa stack->length.
 */
int push(HufStack* stack, HufNode* node);



/*  Arranca cabeça da pilha
 *
 *  I: Pilha a ser decapitada
 *
 *  O: Cabeça da pilha se (stack->length > 0) ou NULL.
 *
 *  Obs: Decrementa stack->length.
 */
HufNode* pull(HufStack* stack);



#endif //HUFFMANOCOOL_HUFSTACK_H
