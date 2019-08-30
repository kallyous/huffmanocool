#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "include/globals.h"
#include "include/assist.h"
#include "include/packing.h"
#include "include/stdoutdebug.h"
#include "include/fileloader.h"
#include "include/huffmanstructs.h"
#include "include/bytetable.h"



unsigned long packing_routine()
{
    // Carrega arquivo a compactar no buffer (e seu tamanho em buffer_length)
    byte * buffer;
    unsigned long buffer_length;
    buffer = load_file_into_buffer(FILE_NAME_STR, &buffer_length);

    // Tamanho inicial do arquivo
    printf("Lidos %lu bytes\n", buffer_length);

    // Prepara nome de saida do arquivo compactado
    byte output[strlen(FILE_NAME_STR) + 6];
    sprintf(output, "%s.huff", FILE_NAME_STR);

    // Gera lista encadeada de frequencia de ocorrencia dos bytes
    HufNode* list_head =  gen_list_from_buffer(buffer, buffer_length);

    // Constroi arvore de huffman
    HufNode* tree_root = build_huffman_tree(list_head);

    // Prepara pra montar a string representando a arvore em pre-ordem
    int tree_str_max_legth = 515;// Espaço mais que sufuciente para armazenar os bytes da arvore em string
    byte tree_str[tree_str_max_legth];
    for (int j=0; j < tree_str_max_legth; j++) tree_str[j] = '\0'; // Limpa região

    // Constroi stream em pre-ordem que descreve a arvore
    build_tree_preorder_array(tree_root, tree_str);

    // Armazena o tamanho do stream de bytes que contem a arvore.
    int16_t tree_str_curr_length = strlen(tree_str);
    dfprint("Arvore na notaçao pre-ordem (tamanho %d):\n%s\n\n", tree_str_curr_length, tree_str);

    // Preara tabela descritiva dos bytes e suas versões compactadas
    byte** byte_table = prepare_packing_table();

    // Constroi tabela de bytes compactados
    dfprint("Construindo tabela de bytes compactados...\n\n");
    byte code[9]; // String temporaria para o processo de empacotamento
    strcpy(code, ""); // Começa vazia
    // Viaja pela arvore construindo a tabela de compressao
    build_packing_table(tree_root, byte_table, code);

    // Log estado da tabela
    dump_table(byte_table, "full_table.txt");
    if (validate_table(byte_table)) printf("Tabela não apresenta repetições.\n");

    // Exibe tabela
    if (DEBUG) {
        dfprint("Tabela:\n");
        for (int k=0; k < 256; k++)
            if (strcmp(byte_table[k], "") != 0) dfprint(" %c -> %s\n", k, byte_table[k]);
        dfprint("\n\n"); }

    // Novo buffer pra receber material compactado
    byte* packing_buffer;
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
        byte result[packed_length + 1];
        strcpy(result, byte_stream_into_binary_str(packing_buffer, packed_length));
        printf(" %s\n\n", result); }

    byte* header = build_header(last_byte_garbage, tree_str);

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
    byte file_extension[6];
    int q, i = 0;
    for (q = strlen(FILE_NAME_STR)-5; q < strlen(FILE_NAME_STR); q++)
        file_extension[i++] = FILE_NAME_STR[q];
    file_extension[6] = '\0';
    dfprint("\nExtensão do arquivo de entrada: %s\n", file_extension);
    if (strcmp(file_extension, ".huff") != 0) {
        printf("ERRO: O arquivo %s não apresenta extensão '.huff'\n", FILE_NAME_STR);
        return 0; }
    //------------------------------------------------

    // Carrega arquivo a descompactar no buffer (e seu tamanho em buffer_length)
    byte * buffer;
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
    byte last_byte_garbage = buffer[0] >> 5;
    dfprint("Bits lixo no ultimo byte: %d\n", last_byte_garbage);

    // Cabeçalho: Pega os 13 bits que informam o tamanho da arvore.
    unsigned int part_a = buffer[1];
    unsigned int part_b = buffer[0] & 31U;
    part_b = part_b << 8;
    unsigned int tree_length = part_a | part_b;

    dfprint("Tamanho da árvore: %d\n", tree_length);

    // Lê representação preordem da arvore
    byte tree_str[tree_length+1];
    i = 0; q = 2;
    for (; i < tree_length+1; i++)
        tree_str[i] = buffer[q++];
    tree_str[tree_length] = '\0';
    dfprint("Árvore em pré-ordem:\n%s\n", tree_str);

    // Recria árvore binária
    int index = 0;
    dfprint("Recriando árvore...\n");
    HufNode* tree_root = rebuild_tree_from_str(tree_str, &index, tree_length);
    dfprint("\n");

    // TODO: A partir do primeiro byte após a string da árvore, ler bit a bit e navegar a arvore, descompactando
    FILE* fptr;
    fptr = fopen(FILE_NAME_STR, "wb");

    // Marca avanço na leitura dos bytes do buffer
    unsigned long byte_step;

    // Marca avanço na leitura dos bits
    int bit_step;

    // Vai segurar 0 ou alguma outra coisa, se o bit lido estiver definido
    byte bit_val = 0;

    // Registra contagem de bytes escritos
    unsigned long bytes_written = 0;

    HufNode* curr_node = tree_root;

    int step_reg = 0;

    dfprint("Descompactadno...\n");

    /* Lê, descompacta e já escreve em arquivo.
     * Começamos a ler depois da árvore e dos dois bytes iniciais
     * Não vamos ler o ultimo one_byte dentro do loop */
    for (byte_step = tree_length + 2U; byte_step < buffer_length; byte_step++)
    {
        // buffer_length-1 é o ultimo byte, onde pode haver bits de lixo
        if (byte_step < buffer_length-1) {
            for (bit_step = 7; bit_step > -1; bit_step--) // Usamos << regressivamente a cada byte
            {
                bit_val = buffer[byte_step] & (1U << bit_step);

                if (bit_val == 0)
                    curr_node = curr_node->left;
                else
                    curr_node = curr_node->right;

                if (!curr_node) {
                    printf("\nErro na descompactação!\n");
                    return 0; }

                /* Se alcançamos uma folha, escrevemos seu one_byte no arquivo final, contamos +1 one_byte escrito
                 * e voltamos para a raiz da árvore. */
                if (!(curr_node->left || curr_node->right)) {
                    dfprint(" %u", curr_node->value);
                    if (++step_reg == 8) {
                        dfprint("\n");
                        step_reg = 0; }
                    fwrite(&(curr_node->value), sizeof(byte), 1, fptr);
                    bytes_written++;
                    curr_node = tree_root; }
                // Caso contrário, apenas continuamos lendo os bits e bytes.
            }
        }
        // No útimo one_byte descartamos os bits de lixo ao não iterarmos neles
        else {
            for (bit_step = 7; bit_step >= last_byte_garbage; bit_step--) // Usamos << regressivamente a cada byte
            {
                bit_val = buffer[byte_step] & (1U << bit_step);

                if (bit_val == 0)
                    curr_node = curr_node->left;
                else
                    curr_node = curr_node->right;

                /* Se alcançamos uma folha, escrevemos seu one_byte no arquivo final, contamos +1 one_byte escrito
                 * e voltamos para a raiz da árvore. */
                if (!(curr_node->left || curr_node->right)) {
                    fwrite(&(curr_node->value), sizeof(byte), 1, fptr);
                    bytes_written++;
                    curr_node = tree_root; }
                // Caso contrário, apenas continuamos lendo os bits e bytes.
            }
        }
    }

    // Completo
    fclose(fptr);

    // Retornamos a quantidade de bytes escritos
    return bytes_written;
}



byte* build_header(unsigned int last_byte_garbage, const byte* tree_str)
{
    unsigned int tree_len = strlen(tree_str);
    byte* header = (byte*)malloc(sizeof(byte) * (tree_len + 2));

    unsigned int g = last_byte_garbage << 5;
    unsigned int a = tree_len >> 8;
    unsigned int b = tree_len & (UINT_MAX - 65280);

    header[0] = a | g;
    header[1] = b;

//    header[0] = last_byte_garbage << 5;
//    header[0] |= 8 >> tree_len;
//    header[1] = tree_len;

    // Copia bytes relevantes de tree_str para dentro do cabeçalho
    for (int i=0; i < tree_len; i++)
        header[i+2] = tree_str[i];

    if (DEBUG) {
        byte* test = byte_stream_into_binary_str(header, tree_len + 2);
        dfprint("Header so far: %s\n", header); }

    return header;
}



byte* compress_byte_stream(const byte* stream, unsigned long stream_length,
        byte* table[], unsigned long * compressed_size, int16_t* garbage_length)
{
    dfprint("compress_byte_stream()::Estado inicial\n");
    dfprint("stream_length: %u\n", stream_length);

    // O tamanho maximo teorico de buffer nunca sera maior que o tamanho de stream
    byte* buffer = (byte*)malloc(sizeof(byte) * stream_length);
    if (!buffer) {
        dfprint("Erro na allocação de %d bytes.\n", stream_length);
        return NULL; }

    // Taca 0 em tudo de buffer
    for (int a=0; a < stream_length; a++) buffer[a] = 0;
    buffer[stream_length-1] = '\0';

    dfprint("Estado inicial do buffer criado:\n");
    dfprint("%s\n", byte_stream_into_binary_str(buffer, stream_length));

    unsigned long sb; // Registra o one_byte sendo lido de stream
    *compressed_size = 1; // Tamanho final do stream compactado de bytes (buffer)

    byte code[9]; // Segura a string de 0/1's sendo escrita durante compactaçao

    unsigned long one_byte = 0; // Registra o one_byte sendo escrito durante compactaçao
    byte bit = 0; // Registra o bit sendo definido durante compactaçao
    byte _1 = 1; // Usado na mask pra definir bits
    byte state;
    int i;

    for (sb=0; sb < stream_length; sb++)
    {
        /*  O valor do one_byte lido de stream[i], quando interpretado como inteiro, eh o indice em table onde esta
         *  a string com a representaçao binaria do one_byte compactado. Entao apenas acessamos o indice e copiamos
         *  o conteudo (uma string de 0/1's) para dentro de code, e o analisamos caracter a caracter. */
        // Nao e necessario essa copia, fazemo-no por clareza na leitura do codigo.
        strcpy(code, table[stream[sb]]);
        for (i=0; i < strlen(code); i++) { // O -1 eh pra ignorar-mos o '\0' no final da string.
            state = 0;
            if (code[i] == '0') {
                state &= ~(_1 << (7 - bit));
                buffer[one_byte] &= ~(_1 << (7 - bit));
            }
            else if (code[i] == '1') {
                state |= (_1 << (7 - bit));
                buffer[one_byte] |= (_1 << (7 - bit));
            }

            // Atualiza qual one_byte de buffer esta sendo escrito atualmente
            if (++bit > 7) { // Passou do ultimo bit do one_byte atual
                one_byte++; // Avança para o proximo byte
                bit = 0; // Primeiro bit do proximo one_byte sera definido no proximo ciclo
                *compressed_size += 1; } // Mais um one_byte sera usado, aumentando o tamanho final em 1
        }
    }


    // Se bit == 0, temos o ultimo one_byte vazio e nenhum bit de lixo
    if (bit == 0) {
        *compressed_size -= 1;
        *garbage_length = 0; }
    else
        *garbage_length = 8 - bit; // Calcula bits de lixo no ultimo byte

    // Adiciona um '\0' apos o ultimo indice de buffer, transformando-o numa string.
    buffer[*compressed_size] = '\0'; // Torna possivel usar strcpy() e sprintf() com o resultado final.

    dfprint("compress_byte_stream()::Resultado\n");
    dfprint("%s\n", byte_stream_into_binary_str(buffer, *compressed_size));

    // Terminado
    return buffer;
}
