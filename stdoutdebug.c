#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>

#include "include/globals.h"
#include "include/stdoutdebug.h"


extern bool DEBUG;

void dfprint(const char * format, ...)
{
    if (DEBUG) {
        va_list arglist; // Declara variavel do tipo va_list
        va_start( arglist, format ); // Aloca memória para a variável arglist
        vprintf( format, arglist ); // Usa função específica para dar print formatado com va_list's
        va_end( arglist ); // Libera memória
    }
}


void load_debug_setting(int argc, char * argv[])
{
    DEBUG = false;
    if (argc > 1) {
        int i;
        for (i = 1; i < argc; i++)
            if (strcmp(argv[i], "--debug") == 0) DEBUG = true;
    }
    dfprint("DEBUG mode set.\n");
}
