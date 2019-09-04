#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "globals.h"
#include "debug.h"
#include "fileio.h"
#include "unpacking.h"


bool validate_name()
{
    byte file_extension[6];
    int q, i = 0;
    for (q = strlen(FILE_NAME_STR)-5; q < strlen(FILE_NAME_STR); q++)
        file_extension[i++] = FILE_NAME_STR[q];
    file_extension[6] = '\0';

    dfprint("\nExtensão do arquivo de entrada: %s\n", file_extension);

    if (strcmp(file_extension, ".huff") != 0) {
        printf("ERRO: O arquivo %s não apresenta extensão '.huff'\n", FILE_NAME_STR);
        return 0; }
    else
        return 1;
}


void rewrite_name()
{
    // Elimina o ".huff" do final do nome
    FILE_NAME_STR[strlen(FILE_NAME_STR)-5] = '\0';

    // Adiciona .des no final, pra diferenciar do arquivo de entrada
    sprintf(FILE_NAME_STR, "%s.des", FILE_NAME_STR);

    printf("Arquivo de saída: %s\n", FILE_NAME_STR);
}


unsigned long unpack()
{
    if (!validate_name()) return 0;

    // Carrega arquivo a descompactar no buffer (e seu tamanho em buffer_length)
    byte * buffer;
    unsigned long long buffer_length;
    buffer = load_file_into_buffer(FILE_NAME_STR, &buffer_length);

    // Define nome de saída
    rewrite_name();

    // Exibe tamanho inicial do arquivo
    printf("Lidos %llu bytes\n", buffer_length);

    // Cabeçalho: Pega os três bits que informam o lixo do último byte
    byte last_byte_garbage = buffer[0] >> 5;

    // Debug
    dfprint("Bits lixo no ultimo byte: %d\n", last_byte_garbage);

    // Cabeçalho: Pega os 13 bits que informam o tamanho da arvore.
    unsigned int part_a = buffer[1];
    unsigned int part_b = buffer[0] & 31U;
    part_b = part_b << 8;
    unsigned int tree_length = part_a | part_b;

    // Debuga árvore no terminal
    dfprint("Tamanho da árvore: %d\n", tree_length);
    if (DEBUG) {
        dfprint("Recriando árvore...\n");
        for (int i=2; i < tree_length +2; i++) dfprint("%c", buffer[i]);
        dfprint("\n"); }

    /* Reconstroi árvore binária
     * +2 é o offset causado pelos dois bytes iniciais que informam o tamanho da árvore
     * e a quantidade de bits de lixo */
    unsigned int tree_byte_arr_index = 2; // Índice inicial da árvore
    HufNode* tree_root = rebuild_tree_from_byte_array(buffer, &tree_byte_arr_index, tree_length+2);

    // Pro debug no terminal
    dfprint("\n");

    // DEBUG: Loga árvore reconstruida
    if (DEBUG) {
        unsigned long temp_buffer_length = 1024;
        unsigned long temp_buffer_load = 0;
        byte temp_buffer[temp_buffer_length];
        dump_huffnode_tree("logs/huffman_tree_rebuilt.log", tree_root, 0, temp_buffer, &temp_buffer_load, temp_buffer_length);
    }

    // Abre arquivo para excrever arquivo descompactado
    FILE* fptr;
    fptr = fopen(FILE_NAME_STR, "wb");

    // Marca avanço na LEITURA dos bytes do BUFFER contendo material compactado
    unsigned long byte_step;

    // Marca avanço na leitura dos bits do byte_step sendo processado
    int bit_step;

    // Vai segurar 0 ou, se o bit lido estiver definido, alguma outra coisa (bit lido na posição bit_step)
    byte bit_val = 0;

    // Registra contagem de bytes escritos
    unsigned long bytes_written = 0;

    /* curr_node será usado para navegar a árvore, enquanto usamos tree_root cada vez que precisarmos
     * retornar à raiz da árvore para descompactar o próximo byte. */
    HufNode* curr_node = tree_root;

    /*  Variável de debug.
     *  Usado para contar quantos bytes foram escritos no stdout durante debug, para inserir uma quebra
     *  de linha. Meramente para melhorar a leitura do debug em console. */
    int step_reg = 0;

    // Debug
    dfprint("Descompactadno...\n");

    /* Lê, descompacta e já escreve em arquivo.
     * Começamos a ler no primeiro byte depois do tamanho da árvore mais dois, que são os dois bytes do começo,
     * os quais informam a quantidade de bits de lixo e o tamanho da árvore. */
    for (byte_step = tree_length + 2U; byte_step < buffer_length; byte_step++)
    {
        // buffer_length-1 é o ultimo byte, onde pode haver bits de lixo. Por isso não processamos ele neste bloco 'if'
        if (byte_step < buffer_length-1)
        {
            for (bit_step = 7; bit_step > -1; bit_step--)
            {
                // Captura valor do bit desejado no passo atual
                bit_val = buffer[byte_step] & (1U << bit_step);

                // Navega ramo à esquerda se for 0
                if (bit_val == 0)
                    curr_node = curr_node->left;

                // Ou, se for qualquer outro valor, navega ramo à direita
                else
                    curr_node = curr_node->right;

                // Durante a descompactação, nunca devemos chegar a NULL
                if (!curr_node) {
                    printf("\nErro na descompactação!\n");
                    return 0; }

                /* Se alcançamos uma folha, escrevemos seu byte no arquivo final, contamos +1 byte escrito
                 * e voltamos para a raiz da árvore. */
                if (!(curr_node->left || curr_node->right))
                {
                    // Debug no terminal: quebra a linha a cada 8 bytes escritos no formato binário (e.g. 010111001)
                    dfprint(" %u", curr_node->value);
                    if (++step_reg == 8) {
                        dfprint("\n");
                        step_reg = 0; }

                    // Escreve o byte atual no arquivo aberto
                    fwrite(&(curr_node->value), sizeof(byte), 1, fptr);

                    // Incremente contador de bytes lidos
                    bytes_written++;

                    // Volta à raíz da árvore para continuar a leitura, pois o próximo bit ja pertence ao próximo byte
                    curr_node = tree_root;
                }
                // Caso não seja uma folha, apenas continua lendo os bits e bytes, navegando pela árvore.

            } // Fim do bloco: for (bit_step = 7; bit_step > -1; bit_step--)

        } // Fim do bloco: if (byte_step < buffer_length-1)

        // No útimo byte descartamos os bits de lixo ao não iterarmos neles
        else {
            for (bit_step = 7; bit_step >= last_byte_garbage; bit_step--)
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

    // Terminado, fecha arquivo
    fclose(fptr);

    // Retornamos a quantidade de bytes escritos
    return bytes_written;
}