#ifndef HUFFMANOCOOL_ASSIST_H
#define HUFFMANOCOOL_ASSIST_H


#include "globals.h"


byte* byte_into_binary_str(byte c);



byte* byte_stream_into_binary_str(byte * stream, unsigned long stream_length);



void dump_table(byte* table[], const byte* file_name);



int validate_table(byte* table[]);



unsigned long dump_to_file(byte* file_name, byte* buffer, unsigned int buffer_length);



byte* append_byte(byte* buffer, byte one_byte, unsigned long buffer_load, unsigned long buffer_length);



#endif //HUFFMANOCOOL_ASSIST_H
