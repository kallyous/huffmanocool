#include <stdlib.h>
#include <stdio.h>

#include "include/globals.h"
#include "include/assist.h"
#include "include/stdoutdebug.h"
#include "include/listbuilder.h"


ListNode* gen_list_from_buffer(char * buffer, unsigned long buffer_length)
{
    // Mensagem de debug
    dfprint("gen_list_from_buffer() connectado!\n");

    // Certifica tamanho do buffer
    if (buffer_length < 1) {
        dfprint("ERRO: gen_list_from_buffer() : recebido length_buffer == 0\n");
        return NULL; }

    // Certifica ponteiro do buffer
    if (!buffer) {
        dfprint("ERRO: gen_list_from_buffer() : recebido buffer == NULL\n");
        return NULL; }

    // Prepara array a segurar contagem previa
    int i;
    int arr_length = 256;
    char bytes_count[arr_length];

    // Zera todos os valores da array
    for (i=0; i < arr_length; i++) {
        bytes_count[i] = 0; }

    // DEBUG: Itera byte a byte pelo buffer, exibindo os bytes para testes
    if (DEBUG) {
        dfprint("\n");
        unsigned int j, l=0;
        for (i=0; i < buffer_length; i++) {
            dfprint(" %s", char_into_binary_str(buffer[i]));
            l += 1;
            if (l == 8) {
                l = 0;
                dfprint("\n"); }
        }
        dfprint("\n\n"); }

    // Itera buffer contando ocorrencias de cada byte
    unsigned char b;
    for (i=0; i < buffer_length; i++) {
        b = buffer[i];
        bytes_count[b]++; }

    // DEBUG: Exibe resultado da contagem
    if (DEBUG) {
        char * word;
        for (i=0; i < arr_length; i++) {
            if (bytes_count[i] > 0) {
                word = char_into_binary_str( (char)i );
                dfprint(" %s : %d\n", word, bytes_count[i]); } }
        dfprint("\n"); }

    return NULL;
}