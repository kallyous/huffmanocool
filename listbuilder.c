#include <stdlib.h>
#include <stdio.h>

#include "include/globals.h"
#include "include/stdoutdebug.h"
#include "include/listbuilder.h"


ListNode* gen_list_from_buffer(unsigned char * buffer, unsigned long buffer_length)
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
    unsigned char bytes_count[arr_length];

    // Zera todos os valores da array
    for (i=0; i < arr_length; i++) {
        bytes_count[i] = 0; }

    // Itera byte a byte pelo buffer, contando a ocorrencia dos bytes
    unsigned int j;
    for (i=0; i < buffer_length; i++) {
        j = buffer[i];
        dfprint(" %u", j);
    }
    dfprint("\n");

    return NULL;
}