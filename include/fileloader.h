#ifndef HUFFMANOCOOL_FILELOADER_H
#define HUFFMANOCOOL_FILELOADER_H



#include <stdbool.h>
#include "globals.h"


byte* load_file_into_buffer(byte * file_path, unsigned long * buffer_length);



int write_from_buffer(byte * file_name, byte * buffer, unsigned long buffer_length);



#endif //HUFFMANOCOOL_FILELOADER_H
