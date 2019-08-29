#ifndef HUFFMANOCOOL_PACKING_H
#define HUFFMANOCOOL_PACKING_H


/*  Reune as operações de compactação de arquivo.
 *
 *  O: a definir
 */
unsigned long packing_routine();



/*  Reune as operações de descompactação de arquivo.
 *
 */
unsigned long unpacking_routine();



/* Comprime dados usando arvore gerada previamente
 *
 *  I: Stream de bytes, buffer a ser alocado e segurar dados comprimidos, comprimento do stream,
 *      table[256][9] com as strings representando a codificaçao de cada one_byte em seu indice.
 *
 *  O: Tamanho do buffer com dados compactados
 *
 *  Obs: Ao terminar, garbage_length tera quantos bits de lixo o ultimo one_byte possui.
 */
unsigned char* compress_byte_stream(const char* stream, unsigned long stream_length, char* table[], unsigned long * compressed_size, int16_t* garbage_length);


/*  Constroi o cabeçalho do arquivo compactado
 *
 *  I: Quantidade de bits de lixo no ultimo one_byte do arquivo compactado e a string contendo a arvore em pre-ordem
 *
 *  O: Cadeia de bytes contendo o cabeçalho
 */
unsigned char* build_header(unsigned int last_byte_garbage, const char* tree_str);



#endif //HUFFMANOCOOL_PACKING_H
