#ifndef HUFFMANOCOOL_STDOUTDEBUG_H
#define HUFFMANOCOOL_STDOUTDEBUG_H


#include "globals.h"



void dfprint(const byte * format, ...);



void load_debug_setting(int argc, byte * argv[]);



unsigned long dump_to_file(byte* file_name, byte* buffer, unsigned int buffer_length);



#endif //HUFFMANOCOOL_STDOUTDEBUG_H
