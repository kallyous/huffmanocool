#include <stdlib.h>
#include <stdbool.h>

#include "include/globals.h"
#include "include/listbuilder.h"
#include "include/treebuilder.h"
#include "include/stdoutdebug.h"


TreeNode* EmptyTreeNode()
{
    TreeNode* n = (TreeNode*)malloc(sizeof(TreeNode));
    n->byte = 0;
    n->weight = 0;
    n->left = NULL;
    n->right = NULL;
    return n;
}


TreeNode* gen_huffman_tree_from_frequency_list(ListNode * head)
{
    TreeNode* root;
    ListNode* ln;

//    // DEBUG: Exibe quem sera linkado
//    if (DEBUG) {
//        ln = head;
//        while (ln->next) { // TODO: Pointer shitness here
//            dfprint(" %d - %d\n", ln->byte, ln->count);
//            ln = ln->next; } // TODO: Pointer shitness here
//        dfprint(" %d - %d\n", ln->byte, ln->count); // Exibe a cauda da lista
//        dfprint("\n");
//    }
//
//    ln = head;
//    while (ln->next) { // TODO: Pointer shitness here
//        // TODO: construir arvore aqui
//        ln = ln->next; // TODO: Pointer shitness here
//    }

    return root;
}
