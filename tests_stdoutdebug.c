#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <criterion/criterion.h>

#include "include/stdoutdebug.h"

/* Definindo Testes:

    Test(group_name, test_name)
    {
        // Setup code
        cr_expect(expected_value, "Mensagem descrtiva da asserção e valor esperado.");
        cr_assert(expected_value, "Mensagem descrtiva da asserção e valor esperado.");
    }

 * O framework Criterion agrupa os testes pelo nome do grupo e executa grupo a grupo.
 * Os grupos são executados em ordem alfabética de seus nomes, definidos por group_name.
 * Para um dado grupo, seus tests são também ordenados e executados na ordem alfabética
 * de seus nomes, definidos por test_name.
 *
 * A diferença entre cr_expect() e cr_assert() é que se o valor esperado de cr_assert()
 * não for obtido, o teste é marcado como falho e o código em execução do teste é
 * encerrado. No caso de cr_expect(), muda apenas que o código continua a ser executado,
 * ainda que já marcado como falho.
 */



/* Testeando: load_debug_setting()
 *  Quando chamado sem argumentos suficiente, determina DEBUG == false .
 */
Test(load_debug_setting, no_arguments)
{
    int _argc = 1;
    int str_size = 32;
    char** _argv = calloc(_argc, sizeof(char*));
    for (int i=0; i < _argc; i++)
    _argv[i] = malloc(str_size);
    strcpy(_argv[0], "tests_stdoutdebug");
    load_debug_setting(_argc, _argv);
    cr_assert(DEBUG == false, "DEBUG mode inactive when no arguments received.");
}


/* Testeando: load_debug_setting()
 *  Quando chamado com argumentos suficientes, mas sem conter '--debug',
 *  determina DEBUG == false .
 */
Test(load_debug_setting, unrelated_arguments)
{
    int _argc = 2;
    int str_size = 32;
    char** _argv = calloc(_argc, sizeof(char*));
    for (int i=0; i < _argc; i++)
        _argv[i] = malloc(str_size);
    strcpy(_argv[0], "tests_stdoutdebug");
    strcpy(_argv[1], "--nope");
    load_debug_setting(_argc, _argv);
    cr_assert(DEBUG == false, "DEBUG mode inactive when unrelated arguments received.");
}


/* Testeando: load_debug_setting()
 *  Quando chamado com argumentos suficientes e um deles é '--debug',
 *  determina DEBUG == true .
 */
Test(load_debug_setting, defining_argument)
{
    int _argc = 3;
    int str_size = 32;
    char** _argv = calloc(_argc, sizeof(char*));
    for (int i=0; i < _argc; i++)
    _argv[i] = malloc(str_size);
    strcpy(_argv[0], "tests_stdoutdebug");
    strcpy(_argv[1], "--nope");
    strcpy(_argv[2], "--debug");
    load_debug_setting(_argc, _argv);
    cr_assert(DEBUG == true, "DEBUG mode active when '--debug' argument received.");
}
