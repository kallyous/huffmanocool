#include <stdlib.h>
#include <stdbool.h>

#include "include/globals.h"
#include "include/listbuilder.h"
#include "include/treebuilder.h"
#include "include/stdoutdebug.h"


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
