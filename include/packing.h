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
unsigned char* compress_byte_stream(const char* stream, unsigned long stream_length, char* table[], unsigned long * compressed_size, int16_t* garbage_length);


#endif //HUFFMANOCOOL_PACKING_H
