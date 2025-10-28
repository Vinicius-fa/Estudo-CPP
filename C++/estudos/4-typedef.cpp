/*
    typedef é uma palavra chave em C++ usada para criar um novo nome (alias) para um tipo de dado existente.
    isso pode tornar o código mais legível e fácil de manter, especialmente quando se trabalha com tipos complexos ou longos.
    ajuda com a portabilidade do código, permitindo que você defina tipos de dados de maneira consistente em diferentes plataformas.
    using funciona da mesma forma que typedef, mas é mais flexível e pode ser usado com templates (C++11 em diante).
    USE APENAS QUANDO TIVER BENEFICIO, NÃO ABUSE!
*/

#include <iostream>
#include <vector> // biblioteca para usar vetores (arrays dinâmicos)

typedef std::vector<std::pair<std::string, int >> pairlist_t;    /*
                                                                    criando um alias para um vetor de pares (string, int)
                                                                    o uso de "_t" no final do nome é uma convenção para indicar que é um tipo
                                                                */ 

typedef std::string text_t; // criando um alias para string 
typedef int number_t; // criando um alias para int

using text_t = std::string; // outra forma de criar um alias para string (C++11 em diante)
using number_t = int; // outra forma de criar um alias para int (C++11 em diante)

int main() {

    pairlist_t pairlist; // usando o alias para declarar uma variável
    text_t nome = "Ana"; // usando o alias para declarar uma variável
    number_t idade = 30; // usando o alias para declarar uma variável

    std::cout << "Nome: " << nome << std::endl;
    std::cout << "Ela tem: " << idade << std::endl;

    return 0;
}