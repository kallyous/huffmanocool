#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    FILE* fptr = fopen("assets/ascii.txt", "wb");
    unsigned char one_byte;
    for (one_byte=0;one_byte< 255; one_byte++) fwrite(&one_byte, 1, 1, fptr);
    one_byte= 255;
    fwrite(&one_byte, 1, 1, fptr);
    fclose(fptr);
    return 0;
}