#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "include/globals.h"
#include "include/assist.h"
#include "include/stdoutdebug.h"
#include "include/argdefs.h"
#include "include/fileloader.h"
#include "include/huffmanstructs.h"
#include "include/chartable.h"



int main(int argc, char * argv[])
{
    // Interpreta e valida argumentos recebidos
    if (!parse_arguments(argc, argv)) return 255;

    // Boas vindas
    printf("Huffman O'cool XVIII é refresco!\n");

    // Carrega arquivo a compactar no buffer (e seu tamanho em buffer_length)
    char * buffer;
    unsigned long buffer_length;
    buffer = load_file_into_buffer(FILE_NAME_STR, &buffer_length);

    // Gera lista encadeada de frequencia de ocorrencia dos bytes
    HufNode* list_head =  gen_list_from_buffer(buffer, buffer_length);

    // Constroi arvore de huffman
    HufNode* tree_root = build_huffman_tree(list_head);


    // TODO: Mover esta gandaia para o lugar adequado

    // Constroi string contendo representaçao da arvore.

    // Tamanho maximo que uma string da arvore pode chegar, mais o caracter '\0' no final.
    int tree_str_max_legth = 515;// Se tivermos '*' e '\', teremos que gastar mais dois bytes pra escapar cada um.
    char tree_str[tree_str_max_legth];
    for (int j=0; j < tree_str_max_legth; j++) tree_str[j] = '\0'; // Limpa toda area da memoria
    build_tree_preorder_array(tree_root, tree_str);
    int tree_str_curr_length = strlen(tree_str) - 1; // Exclui o '\0' da contagem, pois nao entra no cabeçalho.
    dfprint("Arvore na notaçao pre-ordem (tamanho %d):\n%s\n\n", tree_str_curr_length, tree_str);

    // Tabela de compressao
    int byte_table_length = 256;
    char* byte_table[byte_table_length];
    for (int i=0; i < byte_table_length; i++) {
        byte_table[i] = (char*)malloc(sizeof(char)*9);
        strcpy(byte_table[i], ""); }
    char code[9];
    strcpy(code, "");

    // Constroi tabela de bytes compactados
    dfprint("Construindo tabela de bytes compactados...\n");
    build_packing_table(tree_root, byte_table, code);
    // TODO: Fim da gandaia


    return 0;
}
