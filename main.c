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

    // Prepara pra montar a string representando a arvore em pre-ordem
    int tree_str_max_legth = 515;// Espaço mais que sufuciente para armazenar os bytes da arvore em string
    char tree_str[tree_str_max_legth];
    for (int j=0; j < tree_str_max_legth; j++) tree_str[j] = '\0'; // Limpa região

    // Constroi stream em pre-ordem que descreve a arvore
    build_tree_preorder_array(tree_root, tree_str);

    // Armazena o tamanho do stream de bytes que contem a arvore.
    int16_t tree_str_curr_length = strlen(tree_str);
    dfprint("Arvore na notaçao pre-ordem (tamanho %d):\n%s\n\n", tree_str_curr_length, tree_str);

    // Preara tabela descritiva dos bytes e suas versões compactadas
    char** byte_table = prepare_packing_table();

    // Constroi tabela de bytes compactados
    dfprint("Construindo tabela de bytes compactados...\n\n");
    char code[9]; // String temporaria para o processo de empacotamento
    strcpy(code, ""); // Começa vazia
    // Viaja pela arvore construindo a tabela de compressao
    build_packing_table(tree_root, byte_table, code);

    // Exibe tabela
    if (DEBUG) {
        dfprint("Tabela:\n");
        for (int k=0; k < 256; k++)
            if (strcmp(byte_table[k], "") != 0) dfprint(" %c -> %s\n", k, byte_table[k]);
        dfprint("\n\n"); }

    // Novo buffer pra receber material compactado
    unsigned char* packing_buffer;
    // Armazena tamanho do buffer depois de compactado
    unsigned long packed_length;
    // Armazena quantos bits de lixo ficou no ultimo byte
    int16_t last_byte_garbage;

    // Comprime buffer para dentro de packing_buffer e armazena o tamanho final do stream de bytes.
    packing_buffer = compress_byte_stream(buffer, buffer_length, byte_table, &packed_length, &last_byte_garbage);

    // Olhadinha no estado atual
    if (DEBUG) {
        dfprint("Tamanho compactado: %u\n", packed_length);
        dfprint("Bits de lixo: %d\n", last_byte_garbage);
        unsigned char result[packed_length + 1];
        strcpy(result, byte_stream_into_binary_str(packing_buffer, packed_length));
        printf(" %s\n\n", result); }

    char* header = build_header(last_byte_garbage, tree_str);

    // Escreve arquivo
    FILE* fptr;
    fptr = fopen("bin/output.huff", "wb");
    fwrite(header, strlen(tree_str)+2, 1, fptr);
    fwrite(packing_buffer, packed_length, 1, fptr);

    fclose(fptr);

    return 0;
}
