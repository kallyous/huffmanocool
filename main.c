#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "globals.h"
#include "assist.h"
#include "debug.h"
#include "argdefs.h"
#include "fileio.h"
#include "huffmanstructs.h"
#include "bytetable.h"
#include "packing.h"
#include "unpacking.h"


int main(int argc, byte * argv[])
{
    // Interpreta e valida argumentos recebidos
    if (!parse_arguments(argc, argv)) return 255;

    unsigned long final_length;

    // Boas vindas
    printf("-------------------------------------------------------------------------\n");
    printf("Huffman O'cool XVIII é refresco!\n");
    printf("-------------------------------------------------------------------------\n\n");

    // Verifica opção escolhida pelo usuário e chama rotina adequada
    if (OPERATION == 'P') {
        dfprint("Rotina de compactação escolhida.\n");
        printf("Compactadno %s ...\n", FILE_NAME_STR);
        final_length = pack(); }
    else if (OPERATION == 'U') {
        dfprint("Rotina de descompactação escolhida.\n");
        printf("Descompactadno %s ...\n", FILE_NAME_STR);
        final_length = unpack(); }
    else {
        dfprint("Rotina não definida, encerrando...\n");
        return 255; }

    printf("\nEscrito %lu bytes.\n", final_length);
    printf("-------------------------------------------------------------------------\n");

    return 0;
}
