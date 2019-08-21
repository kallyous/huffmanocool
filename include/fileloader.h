#ifndef HUFFMANOCOOL_FILELOADER_H
#define HUFFMANOCOOL_FILELOADER_H



#include <stdbool.h>


char* load_file_into_buffer(char * file_path, unsigned long * buffer_length);



int write_from_buffer(char * file_name, char * buffer, unsigned long buffer_length);



#endif //HUFFMANOCOOL_FILELOADER_H
