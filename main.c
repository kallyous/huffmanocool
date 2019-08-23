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
#include "include/packing.h"


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

    // Limpa toda area da memoria
    for (int j=0; j < tree_str_max_legth; j++) tree_str[j] = '\0';

    // Constroi string em pre-ordem que descreve a arvore
    build_tree_preorder_array(tree_root, tree_str);

    // Armazena o tamanho do stream de bytes que contem a arvore.
    // Exclui o '\0' da contagem, pois nao entra-ra no cabeçalho.
    int16_t tree_str_curr_length = strlen(tree_str);

    dfprint("Arvore na notaçao pre-ordem (tamanho %d):\n%s\n\n", tree_str_curr_length, tree_str);


    // Tabela de compressao

    // Qtd de valores possiveis de se representar com 1 byte
    int byte_table_length = 256;
    // Cada indice segura a representaçao binaria de uma folha, em string de 0/1's
    char* byte_table[byte_table_length];

    // Limpa todas as strings
    for (int i=0; i < byte_table_length; i++) {
        byte_table[i] = (char*)malloc(sizeof(char)*9);
        strcpy(byte_table[i], ""); }

    // TODO: Fim da gandaia


    // Constroi tabela de bytes compactados
    dfprint("Construindo tabela de bytes compactados...\n\n");
    char code[9]; // String temporaria para o processo de empacotamento
    strcpy(code, ""); // Começa vazia
    // Viaja pela arvore construindo a tabela de compressao
    build_packing_table(tree_root, byte_table, code);

    // Exibe resultado apenas para valores definidos
    if (DEBUG) {
        dfprint("Tabela:\n");
        for (int k=0; k < byte_table_length; k++)
            if (strcmp(byte_table[k], "") != 0) dfprint(" %c -> %s\n", k, byte_table[k]);
        dfprint("\n\n"); }

    // Em novo buffer, taca material compactado
    unsigned char* packing_buffer;
    // Tamanho em bytes dos dados compactados
    unsigned long packed_length;
    // Armazena quantos bits de lixo ficou no ultimo byte
    int16_t last_byte_garbage;

    // Comprime buffer para dentro de packing_buffer e armazena o tamanho final do stream de bytes.
    packing_buffer = compress_byte_stream(buffer, buffer_length, byte_table, &packed_length, &last_byte_garbage);

    if (DEBUG) {
        dfprint("\nTamanho compactado: %u\n", packed_length);
        dfprint("Bits de lixo: %d\n\n", last_byte_garbage);
        unsigned char result[packed_length + 1];
        strcpy(result, byte_stream_into_binary_str(packing_buffer, packed_length));
        printf(" %s\n\n", result); }

    // Prepara cabeçalho
    u_int16_t header_meta = 0;
    last_byte_garbage = last_byte_garbage << 13;
    header_meta |= last_byte_garbage;
    header_meta |= tree_str_curr_length;

    // Descarrega tudo em arquivo:
    FILE* fptr;
    fptr = fopen("bin/output.huff", "wb");
    fwrite(&header_meta, sizeof(header_meta), 1, fptr);

    fclose(fptr);

    return 0;
}
