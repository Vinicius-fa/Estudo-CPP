 /*
    namespace é um espaço de nomes que agrupa identificadores (nomes de variáveis, funções, classes, etc) sob um nome comum, evitando conflitos de nomes.
    Em C++, o namespace padrão é o "std", que contém a biblioteca padrão do C++ (como cout, cin, string, vector, etc).
    Usar namespaces ajuda a organizar o código e evitar colisões de nomes, especialmente em projetos grandes
    ou quando se utiliza bibliotecas de terceiros.
*/


#include <iostream>

namespace first {
    int x = 1;
}

namespace second {
    int x = 2;
}

int main() {
   
    int x = 0;

    std::cout << x << std::endl;
    std::cout << first::x << std::endl; // acessando x do namespace first
    std::cout << second::x << std::endl; // acessando x do namespace second

    return 0;
}