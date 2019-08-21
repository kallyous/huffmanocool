#include <stdlib.h>
#include <stdio.h>

#include "include/globals.h"
#include "include/assist.h"
#include "include/stdoutdebug.h"
#include "include/listbuilder.h"
#include "include/sort.h"


ListNode *EmptyListNode()
{
    ListNode * n = (ListNode *)malloc(sizeof(ListNode));
    n->byte = 0;
    n->count = 0;
    n->next = NULL;
    return n;
}


ListNode * FillListNode(char value, unsigned long frequency)
{
    ListNode * n = EmptyListNode();
    n->byte = value;
    n->count = frequency;
    return n;
}


ListNode* gen_list_from_buffer(char * buffer, unsigned long buffer_length)
{
    // Mensagem de debug
    dfprint("gen_list_from_buffer() connectado!\n");

    // Certifica tamanho do buffer
    if (buffer_length < 1) {
        dfprint("ERRO: gen_list_from_buffer() : recebido length_buffer == 0\n");
        return NULL; }

    // Certifica ponteiro do buffer
    if (!buffer) {
        dfprint("ERRO: gen_list_from_buffer() : recebido buffer == NULL\n");
        return NULL; }

    // Prepara array a segurar elementos durante contagem
    int i;
    int arr_length = 256;
    ListNode * node_array[arr_length];

    // Zera todos os valores da array
    for (i=0; i < arr_length; i++) {
        node_array[i] = FillListNode(i, 0); }

    // DEBUG: Itera byte a byte pelo buffer, exibindo os bytes para testes
    if (DEBUG) {
        dfprint("\n");
        unsigned int j, l=0;
        for (i=0; i < buffer_length; i++) {
            dfprint(" %s", char_into_binary_str(buffer[i]));
            l += 1;
            if (l == 8) {
                l = 0;
                dfprint("\n"); }
        }
        dfprint("\n\n"); }

    // Itera buffer contando ocorrencias de cada byte
    unsigned char b;
    for (i=0; i < buffer_length; i++) {
        b = buffer[i];
        node_array[b]->count++; }

    // DEBUG: Exibe resultado da contagem
    if (DEBUG) {
        char * word;
        for (i=0; i < arr_length; i++) {
            if (node_array[i]->count > 0) {
                word = char_into_binary_str( (char)node_array[i]->byte );
                dfprint(" %s : %d\n", word, node_array[i]->count); } }
        dfprint("\n"); }

    // TODO: Ordena array
    bubble_sort(node_array, arr_length);
    // DEBUG: Exibe rersultado da ordenaçao
    if (DEBUG) {
        char * word;
        for (i=0; i < arr_length; i++) {
            if (node_array[i]->count > 0) {
                word = char_into_binary_str( (char)node_array[i]->byte );
                dfprint(" %s : %d\n", word, node_array[i]->count); } }
        dfprint("\n"); }

    // TODO: Conecta vizinhos

    return NULL;
}