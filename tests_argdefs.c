#include <stdlib.h>
#include <string.h>
#include <criterion/criterion.h>

#include "include/argdefs.h"



Test(argdefs, parse_arguments__too_few_arguments)
{
    int argc = 1;
    int size_str = 64;
    byte ** argv = calloc(argc, sizeof(byte*));
    for (int i=0; i < argc; i++) argv[i] = malloc(size_str);
    strcpy(argv[0], "bin/huffmanocool");

    bool result = parse_arguments(argc, argv);

    cr_assert(!result, "parse_arguments() shall return false to too few arguments");
}


Test(argdefs, parse_arguments__bad_arguments)
{
    int argc = 3;
    int size_str = 64;
    byte ** argv = calloc(argc, sizeof(byte*));
    for (int i=0; i < argc; i++) argv[i] = malloc(size_str);
    strcpy(argv[0], "bin/huffmanocool");
    strcpy(argv[1], "--nope");
    strcpy(argv[2], "nupe");

    bool result = parse_arguments(argc, argv);

    cr_assert(!result, "parse_arguments() shall return false to useless arguments");
}


Test(argdefs, parse_arguments__broken_arguments)
{
    int argc = 2;
    int size_str = 64;
    byte ** argv = calloc(argc, sizeof(byte*));
    for (int i=0; i < argc; i++) argv[i] = malloc(size_str);
    strcpy(argv[0], "bin/huffmanocool");
    strcpy(argv[1], "--file-path");

    bool result = parse_arguments(argc, argv);

    cr_assert(!result, "parse_arguments() shall return false to incomplete arguments chain");
}


Test(argdefs, parse_arguments__good_arguments)
{
    int argc = 3;
    int size_str = 64;
    byte ** argv = calloc(argc, sizeof(byte*));
    for (int i=0; i < argc; i++) argv[i] = malloc(size_str);
    strcpy(argv[0], "bin/huffmanocool");
    strcpy(argv[1], "--file-path");
    strcpy(argv[2], "assets/girl-heavy-armor.jpg");

    bool result = parse_arguments(argc, argv);

    cr_assert(result, "parse_arguments() shall return true to correct arguments");
}