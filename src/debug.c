#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>

#include "globals.h"
#include "debug.h"
#include "assist.h"
#include "huffmanstructs.h"


extern bool DEBUG;


void dfprint(const byte * format, ...)
{
    if (DEBUG) {
        va_list arglist; // Declara variavel do tipo va_list
        va_start( arglist, format ); // Aloca memória para a variável arglist
        vprintf( format, arglist ); // Usa função específica para dar print formatado com va_list's
        va_end( arglist ); // Libera memória
    }
}


void load_debug_setting(int argc, byte * argv[])
{
    DEBUG = false;
    if (argc > 1) {
        int i;
        for (i = 1; i < argc; i++)
            if (strcmp(argv[i], "--debug") == 0) DEBUG = true;
    }
    dfprint("DEBUG mode set.\n");
}


unsigned long dump_to_file(const byte* file_name, const byte* buffer,
   unsigned long buffer_start, unsigned long buffer_end)
{
    FILE* fptr = fopen(file_name, "wb");
    unsigned long result = fwrite(&(buffer[buffer_start]), sizeof(byte), buffer_end-buffer_start, fptr);
    fclose(fptr);
    return result;
}


void dump_table(byte* table[], const byte* file_name)
{
    printf("\n%s\n", file_name);
    FILE* fptr = fopen(file_name, "wb");
    if (fptr)
    {
        byte line[16];
        for (int a=0; a < 16; a++) line[a] = '\0';
        for (int b=0; b < 256; b++)
        {
            sprintf(line, "%d\t%s\n", b, table[b]);
            fputs(line, fptr);
        }
    }
    fclose(fptr);
}


byte* dump_huffnode_tree(byte* file_name, HufNode* node, int level, byte* buffer, unsigned long * buffer_load,
    unsigned int buffer_length)
{
    // Detecta passagem de folha e retorna
    if (!node) return buffer;

    // Detecta raiz e limpa buffer
    if (level < 1) {
        level = 0;
        *buffer_load = 0;
        for (int i=0; i < buffer_length; i++) buffer[i] = '\0'; }

    // Põe byte/char atual em buffer
    append_byte(buffer, node->value, *buffer_load, buffer_length);
    *buffer_load += 1;

    dump_huffnode_tree(file_name, node->left, level+1, buffer, buffer_load, buffer_length);
    dump_huffnode_tree(file_name, node->right, level+1, buffer, buffer_load, buffer_length);

    if (level == 0) dump_to_file(file_name, buffer, 0, *buffer_load);

    return buffer;
}