#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "globals.h"
#include "debug.h"
#include "fileio.h"
#include "unpacking.h"


unsigned long unpack()
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
    dump_huffnode_tree("logs/huffman_tree_rebuilt.log", tree_root, 0, temp_buffer, &temp_buffer_load, temp_buffer_length);

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