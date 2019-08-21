#include <stdlib.h>
#include <stdio.h>

#include "include/globals.h"
#include "include/assist.h"
#include "include/stdoutdebug.h"
#include "include/huffmanstructs.h"
#include "include/sort.h"



HufNode *EmptyHufNode()
{
    HufNode * n = (HufNode *)malloc(sizeof(HufNode));
    n->byte = 0;
    n->count = 0;
    n->next = NULL;
    n->left = NULL;
    n->right = NULL;
    return n;
}



HufNode * FillHufNode(char value, unsigned long frequency)
{
    HufNode * n = EmptyHufNode();
    n->byte = value;
    n->count = frequency;
    return n;
}



HufNode* link_ordered_array(HufNode * node_arr[], int length)
{
    dfprint("Gerando lista encadeada...\n");

    int i = 0;
    while (node_arr[i]->count < 1) i++;

    HufNode* head = node_arr[i];

    char * word;
    for (; i < length-1; i++) {
        node_arr[i]->next = node_arr[i+1]; // Conecta elo aa cadeia
        word = char_into_binary_str( (char)node_arr[i]->byte );
        dfprint(" %s : %d\n", word, node_arr[i]->count); }
    dfprint(" %s : %d\n\n", word, node_arr[i]->count);

    // DEBUG: Testa conexoes da lista encadeada
    if (DEBUG) {
        dfprint("Testando segmentaçao da lista encadeada...\n");
        HufNode* n = head;
        while (n->next) {
            dfprint(" %s -> %d\n", char_into_binary_str((char)n->byte), n->count);
            n = n->next; }
        dfprint(" %s -> %d\n\n", char_into_binary_str((char)n->byte), n->count); }

    return head;
}



HufNode* gen_list_from_buffer(char * buffer, unsigned long buffer_length)
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
    HufNode * node_array[arr_length];

    // Zera todos os valores da array
    for (i=0; i < arr_length; i++) {
        node_array[i] = FillHufNode(i, 0); }

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

    // Ordena array
    bubble_sort(node_array, arr_length);

    // DEBUG: Exibe rersultado da ordenaçao
    if (DEBUG) {
        char * word;
        for (i=0; i < arr_length; i++) {
            if (node_array[i]->count > 0) {
                word = char_into_binary_str( (char)node_array[i]->byte );
                dfprint(" %s : %d\n", word, node_array[i]->count); } }
        dfprint("\n"); }

    // Gera lista encadeada a partir da array ordenada
    HufNode* head = link_ordered_array(node_array, arr_length);

    return head;
}



HufNode* build_huffman_tree(HufNode * head)
{
    int z = 0;

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
                dfprint(" %d - %d\n", dhead->byte, dhead->count);
                dhead = dhead->next; }
            dfprint(" %d - %d\n", dhead->byte, dhead->count); // Exibe a cauda da lista
            dfprint("\n"); }

        // Prepara
        HufNode* new = EmptyHufNode();
        left = list_head;
        right = list_head->next;

        // Calcula novo nodo
        new->byte = '*';
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
        {   /*  Loop atual ecerra ao alcançar uma dentre estas tres possibilidades:
             *
           #1-> !next_node->next implica que new->count eh o maior de todos.
             *      Devemos posicionar new no final da lista, aa direita de next_node
             *
             *  !(new->count > next_node->count) implica as outras duas possibilidades:
           #2->     new->count < next_node->count
             *          Devemos posicionar new aa esquerda de next_node
             *
           #3->     new->count == next_node->count
             *          Devemos posicionar new aa esquerda de next_node
             */

            // Navega adiante
            if (next_node->next) {
                prev_node = next_node; // Lembra o nodo imediatamente anterior ao proxima a analizar.
                next_node = next_node->next; }

                // Chegamos ao final da fila e new tem o maior count
            else {
                next_node->next = new; // Coloca new aa direita de next_node
                break; } // Iteraçao atual concluida.
        }

        // Se next_node->next == new, ja re-inserimos new na lista. Atualize head e prossiga.
        if (next_node->next == new) {
            list_head = right->next;
            continue; }

        // Se chegamos aqui, ou (new->count == next_node->count) ou (new->count < next_node->count)
        new->next = next_node; // Insere new aa esquerda de next_node

        // Se next_node ainda eh right->next, nada foi navegado pois new foi menor que o elemento seguinte
        if (next_node == right->next)
            list_head = new; // Entao new eh a nova cabeça
        else { // Caso contrario, new ta depois de prev_node e antes de next_node
            prev_node->next = new; // prev_node agora aponta para new que por sua vez ja aponta para next_node
            list_head = right->next; } // Atualiza head
    }

    dfprint("build_huffman_tree() :: Sucesso na criaçao da arvore!\n");
    return root;
}
