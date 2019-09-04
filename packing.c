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
#include "include/huffmanstructsdebug.h"


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
        dump_table(byte_table, "full_table.log");
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

    // Recria árvore binária
    unsigned int tree_byte_arr_index = 2;
    if (DEBUG) {
        dfprint("Recriando árvore...\n");
        for (int i=tree_byte_arr_index; i < tree_length +2; i++) dfprint("%c", buffer[i]);
        dfprint("\n"); }

    HufNode* tree_root = rebuild_tree_from_byte_array(buffer, &tree_byte_arr_index, tree_length+2); // +2 é o offset causado pelos dois bytes iniciais que informam o tamanho da árvore e a quantidade de bits de lixo

    dfprint("\n");

    // Loga árvore reconstruida
    unsigned long temp_buffer_length = 1024;
    unsigned long temp_buffer_load = 0;
    byte temp_buffer[temp_buffer_length];
    dump_huffnode_tree("huffman_tree_rebuilt.log", tree_root, 0, temp_buffer, &temp_buffer_load, temp_buffer_length);

    // Abre arquivo para excrever arquivo descompactado
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

    // Será usado para navegar a árvore
    HufNode* curr_node = tree_root;

    /*  Variável de debug.
     *  Usado para contar quantos bytes foram escritos no stdout durante debug, para inserir uma quebra
     *  de linha. Meramente para melhorar a leitura do debug em console. */
    int step_reg = 0;

    dfprint("Descompactadno...\n");

    /* Lê, descompacta e já escreve em arquivo.
     * Começamos a ler depois da árvore e dos dois bytes iniciais. */
    for (byte_step = tree_length + 2U; byte_step < buffer_length; byte_step++)
    {
        // buffer_length-1 é o ultimo byte, onde pode haver bits de lixo
        if (byte_step < buffer_length-1)
        {
            for (bit_step = 7; bit_step > -1; bit_step--) // Usamos << a cada byte
            {
                // Captura valor do bit desejado no passo atual
                bit_val = buffer[byte_step] & (1U << bit_step);

                // Navega à esquerda se for 0
                if (bit_val == 0)
                    curr_node = curr_node->left;

                // Navega à esquerda se for qualquer outro valor
                else
                    curr_node = curr_node->right;

                // Durante a descompactação, nunca devemos chegar a NULL.
                if (!curr_node) {
                    printf("\nErro na descompactação!\n");
                    return 0; }

                /* Se alcançamos uma folha, escrevemos seu byte no arquivo final, contamos +1 byte escrito
                 * e voltamos para a raiz da árvore. */
                if (!(curr_node->left || curr_node->right))
                {
                    // Formatação de debug no stdout
                    dfprint(" %u", curr_node->value);
                    if (++step_reg == 8) {
                        dfprint("\n");
                        step_reg = 0; }

                    // Escreve o byte atual no arquivo aberto
                    fwrite(&(curr_node->value), sizeof(byte), 1, fptr);

                    // Incremente contador de bytes lidos
                    bytes_written++;

                    // Volta à raíz da árvore para continuar a leitura
                    curr_node = tree_root;
                }
                // Caso não seja uma folha, apenas continua lendo os bits e bytes, navegando pela árvore.

            } // for (bit_step = 7; bit_step > -1; bit_step--)

        } // if (byte_step < buffer_length-1)

        // No útimo byte descartamos os bits de lixo ao não iterarmos neles
        else {
            for (bit_step = 7; bit_step >= last_byte_garbage; bit_step--) // Usamos << a cada byte
            {
                // Captura valor do bit desejado
                bit_val = buffer[byte_step] & (1U << bit_step);

                // Navega à esquerda se for zero
                if (bit_val == 0)
                    curr_node = curr_node->left;

                // Navega à direita se for outro valor
                else
                    curr_node = curr_node->right;

                // Ao alcançarmos a última folha, repetimos os mesmos procedimentos de antes e escrevemos o byte
                if (!(curr_node->left || curr_node->right))
                {
                    // Escreve byte da folha
                    fwrite(&(curr_node->value), sizeof(byte), 1, fptr);

                    // Incrementa contador de bytes lidos
                    bytes_written++;

                    // Volta à raiz da árvore (ainda que irrelevante, pois acabamos)
                    curr_node = tree_root;
                }
                // Caso contrário, apenas continuamos lendo os bits e bytes.

            } // for (bit_step = 7; bit_step >= last_byte_garbage; bit_step--)

        } // else

    } // for (byte_step = tree_length + 2U; byte_step < buffer_length; byte_step++)

    // Terminado
    fclose(fptr);

    // Retornamos a quantidade de bytes escritos
    return bytes_written;
}



byte* build_header(const byte* tree_byte_arr, unsigned long tree_byte_arr_length, byte last_byte_garbage)
{
    byte* header = (byte*)malloc(sizeof(byte) * (tree_byte_arr_length + 2));

    unsigned int garbage = last_byte_garbage << 5; // Põe os três bits na extremidade esquerda do byte
    unsigned int part_a = tree_byte_arr_length >> 8; // Elimina primeiro byte e põe o próximo na extremidade direita
    unsigned int part_b = tree_byte_arr_length & (UINT_MAX - 65280); // Pega os bits definidos somente no primeiro byte de tree_byte_arr_length

    // Os bytes além do primeiro são descartados nessa operação
    header[0] = part_a | garbage;
    header[1] = part_b;

    // Copia bytes relevantes de tree_byte_arr para dentro do cabeçalho
    for (unsigned long i=0; i < tree_byte_arr_length; i++)
        header[i+2] = tree_byte_arr[i];

    dump_to_file("header_str.log", header, tree_byte_arr_length+2);

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

            if (code[i] == '0') {
                state &= ~(_1 << (7 - bit));
                buffer[one_byte] &= ~(_1 << (7 - bit)); }

            else if (code[i] == '1') {
                state |= (_1 << (7 - bit));
                buffer[one_byte] |= (_1 << (7 - bit)); }

            // Atualiza qual byte de buffer está sendo escrito atualmente
            if (++bit > 7) { // Passou do último bit do byte atual
                one_byte++; // Avança para o próximo byte
                bit = 0; // Primeiro bit do próximo byte será definido no próximo ciclo
                *compressed_size += 1; } // Mais um byte será usado, aumentando o tamanho final em 1
        }
    }


    // Se bit == 0, temos o ultimo one_byte vazio e nenhum bit de lixo
    if (bit == 0) {
        *compressed_size -= 1;
        *garbage_length = 0; }
    else
        *garbage_length = 8 - bit; // Calcula bits de lixo no ultimo byte

    // Terminado
    return buffer;
}
