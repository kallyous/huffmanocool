#include <stdlib.h>
#include <string.h>

#include "include/globals.h"
#include "include/assist.h"
#include "include/packing.h"
#include "include/stdoutdebug.h"



unsigned char* build_header(unsigned int last_byte_garbage, const char* tree_str)
{
//    u_int16_t header_meta = 0;
//    last_byte_garbage = last_byte_garbage << 13;
//    header_meta |= last_byte_garbage;
//    header_meta |= tree_str_curr_length;

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