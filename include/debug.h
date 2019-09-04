#ifndef HUFFMANOCOOL_DEBUG_H
#define HUFFMANOCOOL_DEBUG_H


#include "globals.h"
#include "huffmanstructs.h"


void dfprint(const byte * format, ...);


void load_debug_setting(int argc, byte * argv[]);


unsigned long dump_to_file(const byte* file_name, const byte* buffer,
   unsigned long buffer_start, unsigned long buffer_end);


void dump_table(byte* table[], const byte* file_name);


byte* dump_huffnode_tree(byte* file_name, HufNode* node, int level, byte* buffer, unsigned long * buffer_load,
    unsigned int buffer_length);


#endif //HUFFMANOCOOL_DEBUG_H
