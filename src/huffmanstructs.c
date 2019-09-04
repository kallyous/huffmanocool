#include <stdlib.h>
#include <stdio.h>

#include "globals.h"
#include "assist.h"
#include "debug.h"
#include "huffmanstructs.h"
#include "sort.h"


HufNode *EmptyHufNode()
{
    HufNode * n = (HufNode *)malloc(sizeof(HufNode));
    n->value = 0;
    n->count = 0;
    n->next = NULL;
    n->left = NULL;
    n->right = NULL;
    return n;
}


HufNode * FillHufNode(byte value, unsigned long frequency)
{
    HufNode * n = EmptyHufNode();
    n->value = value;
    n->count = frequency;
    return n;
}


HufNode* link_ordered_array(HufNode * node_arr[], int length)
{
    int i = 0;

    // Encontra quem será a cabeça da lista
    while (node_arr[i]->count < 1) i++;
    HufNode* head = node_arr[i];

    // Conecta elementos a partir da cabeça
    for (; i < length-1; i++) node_arr[i]->next = node_arr[i+1];

    // Retorrna a cabeça, elemento de menor ocorrência
    return head;
}


HufNode* gen_list_from_buffer(byte * buffer, unsigned long buffer_length)
{
    // Ddebug
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
    HufNode * node_array[arr_length];

    // Zera todos os valores da array
    for (i=0; i < arr_length; i++) {
        node_array[i] = FillHufNode(i, 0); }

    // DEBUG: Navega byte a byte pelo buffer, exibindo os bytes
    if (DEBUG) {
        dfprint("\n");
        unsigned int j, l=0;
        for (i=0; i < buffer_length; i++) {
            dfprint(" %s", byte_into_binary_str(buffer[i]));
            l += 1;
            if (l == 8) {
                l = 0;
                dfprint("\n"); }
        }
        dfprint("\n\n"); }

    // Navega buffer contando ocorrencias de cada byte
    byte b;
    for (i=0; i < buffer_length; i++) {
        b = buffer[i];
        node_array[b]->count++; }

    // DEBUG: Exibe resultado da contagem
    if (DEBUG) {
        byte * word;
        for (i=0; i < arr_length; i++) {
            if (node_array[i]->count > 0) {
                word = byte_into_binary_str( (byte)node_array[i]->value );
                dfprint(" %s : %d\n", word, node_array[i]->count); } }
        dfprint("\n"); }

    // Ordena array (pra uma array de 256 elementos, tanto faz o algorítmo usado)
    bubble_sort(node_array, arr_length);

    // DEBUG: Exibe rersultado da ordenaçao
    if (DEBUG) {
        byte * word;
        for (i=0; i < arr_length; i++) {
            if (node_array[i]->count > 0) {
                word = byte_into_binary_str( (byte)node_array[i]->value );
                dfprint(" %s : %d\n", word, node_array[i]->count); } }
        dfprint("\n"); }

    // Gera lista encadeada a partir da array ordenada
    HufNode* head = link_ordered_array(node_array, arr_length);

    return head;
}


HufNode* build_huffman_tree(HufNode * head)
{
    int z = 0; // Variável para debug

    HufNode* root;
    HufNode* list_head;

    list_head = head;
    HufNode* left;
    HufNode* right;
    HufNode* prev_node;
    while (list_head->next) {

        // DEBUG: Exibe estado da lista durante a construcao
        if (DEBUG) {
            z++;
            dfprint("build_huffman_tree() :: Iteraçao %d - Estado da lista:\n", z);
            HufNode* dhead = list_head;
            while (dhead->next) {
                dfprint(" %d - %d\n", dhead->value, dhead->count);
                dhead = dhead->next; }
            dfprint(" %d - %d\n", dhead->value, dhead->count); // Exibe a cauda da lista
            dfprint("\n"); }

        // Prepara
        HufNode* new = EmptyHufNode();
        left = list_head;
        right = list_head->next;

        // Calcula novo nodo
        new->value = '*';
        new->count = left->count + right->count;
        new->left = left;
        new->right = right;

        // Se nao existe right->next nessa parte do processo, chegamos ao final da fila
        if (!right->next) {
            root = new;
            break; }

        // Navega lista buscando posiçao onde inserir new
        HufNode* next_node = right->next;
        while (new->count > next_node->count)
        {
            /*  Loop atual ecerra ao alcançar uma dentre estas tres possibilidades:
             *
           #1*  !next_node->next implica que new->count eh o maior de todos.
             *      Devemos posicionar new no final da lista, á direita de next_node
             *
             *  !(new->count > next_node->count) implica as outras duas possibilidades:
           #2*     new->count < next_node->count
             *          Devemos posicionar new á esquerda de next_node
             *
           #3*     new->count == next_node->count
             *          Devemos posicionar new á esquerda de next_node */

            // Navega adiante
            if (next_node->next) {
                prev_node = next_node; // Lembra o nodo imediatamente anterior ao proximo a analizar.
                next_node = next_node->next; }

            // Chegamos ao final da fila e new tem o maior count
            else {
                next_node->next = new; // Coloca new á direita de next_node
                break; } // Iteração atual concluida.
        }

        // Se next_node->next == new, já re-inserimos new na lista. Atualize head e prossiga.
        if (next_node->next == new) {
            list_head = right->next;
            continue; }

        // Se chegamos aqui, ou (new->count == next_node->count) ou (new->count < next_node->count)
        new->next = next_node; // Insere new á esquerda de next_node

        // Se next_node ainda é right->next, nada foi navegado pois new foi menor que o elemento seguinte
        if (next_node == right->next)
            list_head = new; // Então new é a nova cabeça

        // Caso contrario, new está depois de prev_node e antes de next_node
        else {
            prev_node->next = new; // prev_node agora aponta para new, que por sua vez já aponta para next_node
            list_head = right->next; } // Atualiza head

    }

    // Debug
    dfprint("build_huffman_tree() :: Sucesso na criaçao da arvore!\n");
    if (DEBUG) {
        unsigned long buffer_length = 4096;
        unsigned long buffer_load = 0;
        byte* buffer = (byte*)malloc(sizeof(byte)*buffer_length);
        dump_huffnode_tree("logs/huffman_tree_orig.log", root, 0, buffer, &buffer_load, buffer_length);
        free(buffer); }

    return root;
}


void build_tree_preorder_array(HufNode* node, byte* buffer, unsigned long * buffer_load, unsigned long buffer_length)
{
    // Condição de saída
    if (!node) return;

    /*  Caso seja uma folha, escapa '*' e '\', colocando um '\' na frente.
     *  A condição desse bloco será verdadeira sempre que estivermos numa folha, pois elas não
     *  possuem left ou right. */
    if (!(node->left || node->right)) {
        if (node->value == '*' || node->value == '\\') {
            dfprint("Folha com %c alcançada, escapando com '\\'.\n", node->value);
            append_byte(buffer, '\\', *buffer_load, buffer_length);
            *buffer_load += 1;
        }
    }

    // Adiciona byte atual na array de bytes
    append_byte(buffer, node->value, *buffer_load, buffer_length);
    *buffer_load += 1;

    // Recursão
    build_tree_preorder_array(node->left, buffer, buffer_load, buffer_length);
    build_tree_preorder_array(node->right, buffer, buffer_load, buffer_length);
}


HufNode* rebuild_tree_from_byte_array(const byte* byte_array, unsigned int* curr_index, unsigned int tree_arr_length)
{
    // Retorna ao percorrer o stream
    if (*curr_index == tree_arr_length) return NULL;

    // Prepara nó/folha
    HufNode* new_node = EmptyHufNode();

    // Pega byte atual e depois incrementa indice atual
    byte curr_byte = byte_array[*curr_index];
    *curr_index += 1;

    // Debuga o caracter atual
    dfprint(" %c", curr_byte);

    // Veremos se estamos em nó ou folha
    if (curr_byte == '*') {
        new_node->left = rebuild_tree_from_byte_array(byte_array, curr_index, tree_arr_length);
        new_node->right = rebuild_tree_from_byte_array(byte_array, curr_index, tree_arr_length);
    }

    // Se curr_byte é um '\\', estamos escapando um * ou \. Pegue próximo caracter
    if (curr_byte == '\\') {
        curr_byte = byte_array[*curr_index];
        *curr_index += 1; }

    // Salva valor atual no nó/folha
    new_node->value = curr_byte;

    // Retorna nó atual
    return new_node;
}