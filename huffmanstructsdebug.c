#include <stdlib.h>
#include <string.h>

#include "include/globals.h"
#include "include/assist.h"
#include "include/huffmanstructs.h"
#include "include/huffmanstructsdebug.h"


byte* dump_huffnode_tree(byte* file_name, HufNode* node, int level, byte* buffer, unsigned long * buffer_load,
        unsigned int buffer_length)
{
    // Detecta passagem de folha e retorna
    if (!node) return buffer;

    // Detecta raiz e limpa buffer
    if (level < 1) {
        level = 0;
        *buffer_load = 0;
        for (int i=0; i < buffer_length; i++) buffer[i] = '\0'; }

    // Põe byte/char atual em buffer
    append_byte(buffer, node->value, *buffer_load, buffer_length);
    *buffer_load += 1;

    dump_huffnode_tree(file_name, node->left, level+1, buffer, buffer_load, buffer_length);
    dump_huffnode_tree(file_name, node->right, level+1, buffer, buffer_load, buffer_length);

    if (level == 0) dump_to_file(file_name, buffer, *buffer_load);

    return buffer;
}