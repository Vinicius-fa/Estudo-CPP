#include <iostream>
#include <string>

int main() {

    std::string nome;

    while(nome.empty()){ // Enquanto a string estiver vazia, o loop continua (while = enquanto)
        std::cout << "Por favor, digite um nome: ";
        std::getline(std::cin, nome);
    }

    std::cout << "O nome escolhido foi: " << nome << std::endl;

    while(1==1) {
        std::cout << "SOCORRO " << nome << "! ESTOU PRESO EM UM LOOP INFINITO" << std::endl;
    }

    return 0;
}