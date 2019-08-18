# Huffman O'cool XVII

##### *"Porque Huffman O'cool XVII é refresco!"*

---

Projeto avaliativo para disciplina Estrutura de Dados - UFAL, 2019

---

### Especificações do Projeto

**Data da apresentação:** 04/09 (Terça-Feira)

**Itens do Projeto:**

* Implementação do Algoritmo de Huffman
* Seminário sobre algoritmo A*
* Comparação e plotagem ABB vs AVL
* Testes unitários em todos os códigos

**Integrantes:** Lucas Carvalho, Jonathas Patrick, Pablo Vinícius

**Repositório:** https://github.com/kallyous/huffmanocool

---

### Requisitos

Framework para testes unitários em C: [Criterion](https://github.com/Snaipe/Criterion)  
Exemplos de uso na pasta `examples`

---

### Testes Unitários

Como definir e rodar testes unitários para este projeto, usando o framework Criterion.

**Definindo Testes:**

    #include <criterion/criterion.h>
    #include "librarytotest.h"
    
    Test(group_name, test_name)
    {
        // Setup code
        cr_expect(expected_value, "Mensagem descrtiva da asserção e valor esperado.");
        cr_assert(expected_value, "Mensagem descrtiva da asserção e valor esperado.");
    }

O framework Criterion agrupa os testes pelo nome do grupo e executa grupo a grupo. Os grupos são executados em ordem alfabética de seus nomes, definidos por group_name. Para um dado grupo, seus tests são também ordenados e executados na ordem alfabética de seus nomes, definidos por test_name.

A diferença entre cr_expect() e cr_assert() é que se o valor esperado de cr_assert() não for obtido, o teste é marcado como falho e o código em execução do teste é encerrado. No caso de cr_expect(), muda apenas que o código continua a ser executado, ainda que já marcado como falho.

**Compilando Testes:**

    $ gcc tests_code.c -o bin/tests_binary_name -lcriterion

* Lembre-se de compilar todos os binários para dentro da basta `bin`, para não commita-los acidentalmente.
* Por razões fora do escopo deste texto, é necessário o argumento `-lcriterion` para o GCC compilar o framework adequadamente.

Note que este texto assume que as definições e implementações das estruturas necessaŕias são feitas fora do arquivo `main.c`, para poderem ser testadas isoladamente. **Sigam este modelo ao longo do projeto**.

**Executando os Testes:**

Se tudo foi codado e compilado corretamente, o binário resultante pode ser executado diretamente com:

    $ bin/tests_binary_name

Os resultados dos testes serão exibidos no terminal em tempo real, à medida que são executados.

---