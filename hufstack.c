#include "include/huffmanstructs.h"
#include "include/hufstack.h"



HufStack* EmptyHufStack()
{
    HufStack* s = (HufStack*)malloc(typeof(HufStack));
    s->length = 0;
    s->head = s->tail = NULL;
    return s;
}



int push(HufStack* stack, HufNode* node)
{
    node->next = stack->head;
    stack->head = node;
    stack->length++;
    return stack->length;
}



HufNode* pull(HufStack* stack)
{
    if (stack->length < 1) return NULL;
    HufNode* node = stack->head;
    stack->head = stack->head->next;
    stack->length--;
    return node;
}