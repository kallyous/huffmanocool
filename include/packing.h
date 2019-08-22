#ifndef HUFFMANOCOOL_PACKING_H
#define HUFFMANOCOOL_PACKING_H



/* Comprime dados usando arvore gerada previamente
 *
 *  I: Stream de bytes, buffer a ser alocado e segurar dados comprimidos, comprimento do stream,
 *      table[256][9] com as strings representando a codificaçao de cada byte em seu indice.
 *
 *  O: Tamanho do buffer com dados compactados
 *
 *  Obs: Ao terminar, garbage_length tera quantos bits de lixo o ultimo byte possui.
 */
unsigned long compress_byte_stream(char* stream, char* buffer, unsigned long stream_length, char* table[], char* garbage_length);


#endif //HUFFMANOCOOL_PACKING_H
