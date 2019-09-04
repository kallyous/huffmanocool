#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "globals.h"
#include "assist.h"
#include "packing.h"
#include "debug.h"
#include "fileio.h"
#include "huffmanstructs.h"
#include "bytetable.h"


unsigned long pack()
{
    // Carrega arquivo a compactar no buffer (e seu tamanho em buffer_length)
    byte * buffer;
    unsigned long long buffer_length;
    buffer = load_file_into_buffer(FILE_NAME_STR, &buffer_length);

    // Tamanho inicial do arquivo
    printf("Lidos %llu bytes\n", buffer_length);

    // Prepara nome de saida do arquivo compactado
    byte output[strlen(FILE_NAME_STR) + 6];
    sprintf(output, "%s.huff", FILE_NAME_STR);

    // Gera lista encadeada de frequencia de ocorrencia dos bytes
    HufNode* list_head =  gen_list_from_buffer(buffer, buffer_length);

    // Constroi arvore de huffman
    HufNode* tree_root = build_huffman_tree(list_head);

    /* Prepara pra montar uma array de bytes representando a arvore em pre-ordem.
     * Essa array vai pro cabeçalho do arquivo compactado. */
    unsigned long tree_byte_arr_length = 4096;// Espaço mais que sufuciente para armazenar os bytes da arvore em string
    unsigned long tree_byte_arr_load = 0;
    byte tree_byte_arr[tree_byte_arr_length];

    // Constroi stream em pre-ordem que descreve a arvore
    build_tree_preorder_array(tree_root, tree_byte_arr, &tree_byte_arr_load, tree_byte_arr_length);

    // Preara tabela descritiva dos bytes e suas versões compactadas
    byte** byte_table = prepare_packing_table();

    // Constroi tabela de bytes compactados
    dfprint("Construindo tabela de bytes compactados...\n\n");
    byte code[9]; // String temporaria para o processo de empacotamento
    strcpy(code, ""); // Começa vazia
    // Viaja pela arvore construindo a tabela de compressao
    build_packing_table(tree_root, byte_table, code);

    // Loga o estado da tabela
    if (DEBUG) {
        dump_table(byte_table, "logs/full_table.log");
        if (validate_table(byte_table)) printf("Tabela não apresenta repetições.\n");
        else printf("Tabela apresenta repetições de folhas (está quebrada)!\n"); }

    // Novo buffer pra receber material compactado
    byte* compressed_buffer;

    // Armazena tamanho do buffer depois de compactado
    unsigned long compressed_buffer_length;

    // Armazena quantos bits de lixo ficou no ultimo byte
    unsigned int last_byte_garbage;

    // Comprime buffer para dentro de packing_buffer e armazena o tamanho final do stream de bytes.
    compressed_buffer = compress_byte_stream(buffer, buffer_length, byte_table, &compressed_buffer_length, &last_byte_garbage);

    // Constrói cabeçalho
    byte* header = build_header(tree_byte_arr, tree_byte_arr_load, last_byte_garbage);
    unsigned int header_length = tree_byte_arr_load + 2; // Tamanho da string da árvore mais os dois bytes do começo

    // Escreve arquivo
    FILE* fptr;
    fptr = fopen(output, "wb");
    fwrite(header, sizeof(byte), header_length, fptr);
    fwrite(compressed_buffer, sizeof(byte), compressed_buffer_length, fptr);
    fclose(fptr);

    // Tamanho compactado
    return header_length + compressed_buffer_length;
}


byte* build_header(const byte* tree_byte_arr, unsigned long tree_byte_arr_length, byte last_byte_garbage)
{
    byte* header = (byte*)malloc(sizeof(byte) * (tree_byte_arr_length + 2));

    unsigned int garbage = last_byte_garbage << 5; // Põe os três bits na extremidade esquerda do byte
    unsigned int part_a = tree_byte_arr_length >> 8; // Elimina primeiro byte e põe o próximo na extremidade direita
    unsigned int part_b = tree_byte_arr_length & (UINT_MAX - 65280U); // Pega os bits definidos somente no primeiro byte de tree_byte_arr_length

    // Os bytes além do primeiro são descartados nessa operação
    header[0] = part_a | garbage;
    header[1] = part_b;

    // Copia bytes relevantes de tree_byte_arr para dentro do cabeçalho
    for (unsigned long i=0; i < tree_byte_arr_length; i++)
        header[i+2] = tree_byte_arr[i];

    // Debug
    dump_to_file("logs/header_str.log", header, 2, tree_byte_arr_length+2);

    return header;
}


byte* compress_byte_stream(const byte* stream, unsigned long stream_length,
        byte* table[], unsigned long * compressed_size, unsigned int * garbage_length)
{
    // O tamanho maximo teorico de buffer nunca sera maior que o tamanho de stream
    byte* buffer = (byte*)malloc(sizeof(byte) * stream_length);
    if (!buffer) {
        dfprint("Erro na allocação de %d bytes.\n", stream_length);
        return NULL; }

    // Taca 0 em tudo de buffer
    for (int a=0; a < stream_length; a++) buffer[a] = 0;

    // Registra o byte sendo lido de stream
    unsigned long sb;

    // Tamanho final do stream compactado de bytes (buffer)
    *compressed_size = 1;

    // Segura a string de 0/1's sendo escrita durante compactaçao
    byte code[9];

    // Registra o one_byte sendo escrito durante compactaçao
    unsigned long one_byte = 0;

    // Registra o bit sendo definido durante compactaçao
    byte bit = 0;

    // Usado na mask pra definir bits
    byte _1 = 1;

    /*  Variável de debug das operações bitwise.
     *  Durante a execução pelo debugger do CLion, rastreamos essa variável para acompanhar o estado do que
     *  está sendo calculado e lido pelas operações binárias. Esta variável não desempenha outra função. */
    byte state;

    /*  Como stream_length contém o tamanho, em bytes, do stream cotendo o arquivo carregado, podemos usar este valor
     *  como limite para a leitura dos bytes sendo compactados. Assim usamos sb para navegar pelo stream de bytes. */
    for (sb=0; sb < stream_length; sb++)
    {
        /*  O valor do byte lido de stream[sb], quando interpretado como inteiro, é o índice em table onde está
         *  a string com a representação binária do byte compactado. Entao apenas acessamos o índice e copiamos
         *  o conteúdo (uma string de 0/1's) para dentro de code, e o analisamos caracter a caracter.
         *  Não é necessária essa cópia, fazemo-na por clareza na leitura do código. */
        strcpy(code, table[stream[sb]]);

        //  Interpreta os 0/1's dentro de code para escrevermos os bits compactados
        for (int i=0; i < strlen(code); i++) {
            state = 0;

            // Define bit desejado como 0
            if (code[i] == '0') {
                state &= ~(_1 << (7 - bit));
                buffer[one_byte] &= ~(_1 << (7 - bit)); }

            // Define bit desejado como 1
            else if (code[i] == '1') {
                state |= (_1 << (7 - bit));
                buffer[one_byte] |= (_1 << (7 - bit)); }

            // Atualiza qual byte de buffer está sendo escrito atualmente
            if (++bit > 7) { // Passou do último bit do byte atual
                one_byte++; // Avança para o próximo byte
                bit = 0; // Será definido o primeiro bit do próximo byte no próximo ciclo
                *compressed_size += 1; } // Mais um byte será usado, aumentando o tamanho final em 1
        }
    }


    // Se bit == 0, temos o ultimo one_byte vazio e nenhum bit de lixo
    if (bit == 0) {
        *compressed_size -= 1; // Isso porque a gente incrementa compressed_size cada vez que enche um byte
        *garbage_length = 0; }
    else
        *garbage_length = 8 - bit; // Calcula bits de lixo no ultimo byte

    // Terminado
    return buffer;
}
