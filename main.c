#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <time.h>


bool DEBUG = false;


void dfprint(const char * format, ...)
{
    if (DEBUG) {
        va_list arglist; // Declara variavel do tipo va_list
        va_start( arglist, format ); // Aloca memória para a variável arglist
        vprintf( format, arglist ); // Usa função específica para dar print formatado com va_list's
        va_end( arglist ); // Libera memória
    }
}


int main(int argc, char * argv[])
{
    if (argc > 1) {
        int i;
        for (i = 1; i < argc; i++)
            if (strcmp(argv[i], "--debug") == 0) DEBUG = true;
    }
    else {
        printf("DEBUG is off (Default).\nCall with \"--debug\" flag.\n");
    }

    dfprint("Fuck yeah %d %d %d!\n", 1, 2, 3);
    return 0;
}
