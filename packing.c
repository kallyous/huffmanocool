#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/globals.h"
#include "include/assist.h"
#include "include/packing.h"
#include "include/stdoutdebug.h"
#include "include/fileloader.h"
#include "include/huffmanstructs.h"
#include "include/chartable.h"



unsigned long packing_routine()
{
    // Carrega arquivo a compactar no buffer (e seu tamanho em buffer_length)
    char * buffer;
    unsigned long buffer_length;
    buffer = load_file_into_buffer(FILE_NAME_STR, &buffer_length);

    // Tamanho inicial do arquivo
    printf("Lidos %lu bytes\n", buffer_length);

    // Prepara nome de saida do arquivo compactado
    char output[strlen(FILE_NAME_STR) + 6];
    sprintf(output, "%s.huff", FILE_NAME_STR);

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
    fptr = fopen(output, "wb");
    fwrite(header, strlen(tree_str)+2, 1, fptr);
    fwrite(packing_buffer, packed_length, 1, fptr);
    fclose(fptr);

    // Tamanho compactado
    return (strlen(tree_str)+2+packed_length);
}


unsigned long unpacking_routine()
{
    // TODO: Levar validação de nome para outro lugar
    char file_extension[6];
    int i = 0;
    for (int q = strlen(FILE_NAME_STR)-5; q < strlen(FILE_NAME_STR); q++)
        file_extension[i++] = FILE_NAME_STR[q];
    file_extension[6] = '\0';
    dfprint("\nExtensão do arquivo de entrada: %s\n", file_extension);
    if (strcmp(file_extension, ".huff") != 0) {
        printf("ERRO: O arquivo %s não apresenta extensão '.huff'\n", FILE_NAME_STR);
        return 0; }
    //------------------------------------------------

    // Carrega arquivo a descompactar no buffer (e seu tamanho em buffer_length)
    char * buffer;
    unsigned long buffer_length;
    buffer = load_file_into_buffer(FILE_NAME_STR, &buffer_length);

    // Elimina o ".huff" do final do nome
    FILE_NAME_STR[strlen(FILE_NAME_STR)-5] = '\0';
    // Adiciona .des no final, pra diferenciar do arquivo de entrada
    sprintf(FILE_NAME_STR, "%s.des", FILE_NAME_STR);
    printf("Arquivo de saída: %s\n", FILE_NAME_STR);

    // Tamanho inicial do arquivo
    printf("Lidos %lu bytes\n", buffer_length);

    // Cabeçalho: Pega os três bits que informam o lixo do último byte
    unsigned char last_byte_garbage = 0;
    last_byte_garbage = buffer[0] & 1U << 7;    // 00000001 << 7 == 10000000
    last_byte_garbage |= buffer[0] & 1U << 6;   // 00000001 << 6 == 01000000
    last_byte_garbage |= buffer[0] & 1U << 5;   // 00000001 << 5 == 00100000
    last_byte_garbage = 5 >> last_byte_garbage; // 5 >> 11100000 == 00000111
    dfprint("Bits lixo no ultimo byte: %d\n", last_byte_garbage);

    // Cabeçalho: Pega os 13 bits que informam o tamanho da arvore.
    unsigned char byte = buffer[0] << 3;
    unsigned int tree_length = byte;
    tree_length = tree_length << 5;
    tree_length |= buffer[1];
    dfprint("Tamanho da árvore: %d\n", tree_length);

    // TODO: Gerar árvore

    // TODO: A partir do primeiro byte após a string da árvore, ler bit a bit e navegar a arvore, descompactando

    return 0; // TODO: Retornar tamanho do arquivo descompactado
}



unsigned char* build_header(unsigned int last_byte_garbage, const char* tree_str)
{
    unsigned int tree_len = strlen(tree_str);
    unsigned char* header = (char*)malloc(sizeof(char) * (tree_len + 2));

    header[0] = last_byte_garbage << 5;
    header[0] |= 8 >> tree_len;
    header[1] = tree_len;

    // Copia bytes relevantes de tree_str para dentro do cabeçalho
    for (int i=0; i < tree_len; i++)
        header[i+2] = tree_str[i];

    if (DEBUG) {
        char* test = byte_stream_into_binary_str(header, tree_len + 2);
        dfprint("Header so far: %s\n", header); }

    return header;
}



unsigned char* compress_byte_stream(const char* stream, unsigned long stream_length,
        char* table[], unsigned long * compressed_size, int16_t* garbage_length)
{
    dfprint("compress_byte_stream()::Estado inicial\n");
    dfprint("stream_length: %u\n", stream_length);

    // O tamanho maximo teorico de buffer nunca sera maior que o tamanho de stream
    unsigned char* buffer = (char*)malloc(sizeof(char) * stream_length);
    if (!buffer) {
        dfprint("Erro na allocação de %d bytes.\n", stream_length);
        return NULL; }

    // Taca 0 em tudo de buffer
    for (int a=0; a < stream_length; a++) buffer[a] = 0;
    buffer[stream_length-1] = '\0';

    dfprint("Estado inicial do buffer criado:\n");
    dfprint("%s\n", byte_stream_into_binary_str(buffer, stream_length));

    unsigned long sb; // Registra o byte sendo lido de stream
    *compressed_size = 1; // Tamanho final do stream compactado de bytes (buffer)

    char code[9]; // Segura a string de 0/1's sendo escrita durante compactaçao

    unsigned long byte = 0; // Registra o byte sendo escrito durante compactaçao
    unsigned char bit = 0; // Registra o bit sendo definido durante compactaçao
    unsigned char _1 = 1; // Usado na mask pra definir bits
    unsigned char state;
    int i;

    for (sb=0; sb < stream_length; sb++)
    {
        /*  O valor do byte lido de stream[i], quando interpretado como inteiro, eh o indice em table onde esta
         *  a string com a representaçao binaria do byte compactado. Entao apenas acessamos o indice e copiamos
         *  o conteudo (uma string de 0/1's) para dentro de code, e o analisamos caracter a caracter. */
        // Nao e necessario essa copia, fazemo-no por clareza na leitura do codigo.
        strcpy(code, table[stream[sb]]);
        for (i=0; i < strlen(code); i++) { // O -1 eh pra ignorar-mos o '\0' no final da string.
            state = 0;
            if (code[i] == '0') {
                state &= ~(_1 << (7 - bit));
                buffer[byte] &= ~(_1 << (7 - bit));
            }
            else if (code[i] == '1') {
                state |= (_1 << (7 - bit));
                buffer[byte] |= (_1 << (7 - bit));
            }

            // Atualiza qual byte de buffer esta sendo escrito atualmente
            if (++bit > 7) { // Passou do ultimo bit do byte atual
                byte++; // Avança para o proximo byte
                bit = 0; // Primeiro bit do proximo byte sera definido no proximo ciclo
                *compressed_size += 1; } // Mais um byte sera usado, aumentando o tamanho final em 1
        }
    }


    // Se bit == 0, temos o ultimo byte vazio e nenhum bit de lixo
    if (bit == 0) {
        *compressed_size -= 1;
        *garbage_length = 0; }
    else
        *garbage_length = 8 - bit; // Calcula bits de lixo no ultimo byte

    // Adiciona um '\0' apos o ultimo indice de buffer, transformando-o numa string.
    buffer[*compressed_size] = '\0'; // Torna possivel usar strcpy() e sprintf() com o resultado final.

    dfprint("compress_byte_stream()::Resultado\n");
    dfprint("%s\n", byte_stream_into_binary_str(buffer, *compressed_size));

    // Terminado. Retorne a quantidade de bytes em uso por buffer
    return buffer;
}