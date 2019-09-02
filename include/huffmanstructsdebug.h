#ifndef HUFFMANOCOOL_HUFFMANSTRUCTSDEBUG_H
#define HUFFMANOCOOL_HUFFMANSTRUCTSDEBUG_H


#include "huffmanstructs.h"


byte* dump_huffnode_tree(byte* file_name, HufNode* node, int level, byte* buffer, unsigned long * buffer_load,
                         unsigned int buffer_length);



#endif //HUFFMANOCOOL_HUFFMANSTRUCTSDEBUG_H
