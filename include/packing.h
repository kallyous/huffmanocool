#ifndef HUFFMANOCOOL_PACKING_H
#define HUFFMANOCOOL_PACKING_H


#include "globals.h"


/*  Reune as operações de compactação de arquivo.
 *
 *  O: a definir
 */
unsigned long pack();


/* Comprime dados usando arvore gerada previamente
 *
 *  I: Stream de bytes, buffer a ser alocado e segurar dados comprimidos, comprimento do stream,
 *      table[256][9] com as strings representando a codificaçao de cada one_byte em seu indice.
 *
 *  O: Tamanho do buffer com dados compactados
 *
 *  Obs: Ao terminar, garbage_length tera quantos bits de lixo o ultimo one_byte possui.
 */
byte* compress_byte_stream(const byte* stream, unsigned long stream_length,
        byte* table[], unsigned long * compressed_size, unsigned int * garbage_length);


/*  Constroi o cabeçalho do arquivo compactado
 *
 *  I: Quantidade de bits de lixo no ultimo one_byte do arquivo compactado e a string contendo a arvore em pre-ordem
 *
 *  O: Cadeia de bytes contendo o cabeçalho
 */
byte* build_header(const byte* tree_byte_arr, unsigned long tree_byte_arr_length, byte last_byte_garbage);


#endif //HUFFMANOCOOL_PACKING_H
