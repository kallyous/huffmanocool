#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "include/globals.h"
#include "include/assist.h"
#include "include/stdoutdebug.h"
#include "include/argdefs.h"
#include "include/fileloader.h"
#include "include/huffmanstructs.h"
#include "include/bytetable.h"
#include "include/packing.h"


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
        final_length = packing_routine(); }
    else if (OPERATION == 'U') {
        dfprint("Rotina de descompactação escolhida.\n");
        printf("Descompactadno %s ...\n", FILE_NAME_STR);
        final_length = unpacking_routine(); }
    else {
        dfprint("Rotina não definida, encerrando...\n");
        return 255; }

    printf("\nEscrito %lu bytes.\n", final_length);
    printf("-------------------------------------------------------------------------\n");

    return 0;
}
