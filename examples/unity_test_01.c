#include <criterion/criterion.h>

Test(simple, test) {
cr_assert(0, "Hello World");
}

/* Compilar e rodar:
 *
 * 0. Certifique-se de ter instalado o Criterion. Siga o link da secção 'Requerimentos' para o
 *    repositório do Criterion.
 * 1. Navegue pelo terminal até a pasta 'examples', onde estão os códigos de exemplos.
 * 2. Crie uma pasta chamada 'bin' dentro da pasta 'examples'.
 * 3. $ gcc unity_test_01.c -o bin/ut01 -lcriterion
 * 4. $ bin/ut01
 *
 * O repositório está configurado para incluir a pasta 'examples' e ignorar a pasta 'examples/bin',
 * por isso coloque os binários dos exemplos dentro de 'examples/bin'. Não queremos binários dentro
 * do repositório, apenas código fonte.
 *
 * O comando em 3 compila o exemplo, linkando a biblioteca do framework Criterion, e salva o resultado
 * na pasta 'examples/bin'.
 *
 * O comando em 4 executa o programa, que por sua vez roda os testes unitários nele contidos.
 */