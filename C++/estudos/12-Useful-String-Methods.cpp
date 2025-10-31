#include <iostream>
#include <string> // Inclui a biblioteca de strings

int main() {

    std::string nome;

    std::cout << "Escreva seu nome: ";
    std::getline(std::cin, nome); // Lê uma linha inteira, incluindo espaços

    if (nome.empty()) { // Verifica se a string está vazia
        std::cout << "Digite alguma coisa!" << std::endl;
    }

    if (nome.length() > 12) { // Verifica o tamanho da string
        std::cout << "Seu nome nao pode ter mais que 12 caracteres!" << std::endl;
    } else {
        std::cout << "ola, " << nome << "!" << std::endl;
    }

    nome.clear(); // Limpa o conteúdo da string

    std::cout << "Nome apos clear(): '" << nome << "'" << std::endl;

    std::cout << "Escreva seu nome: ";
    std::getline(std::cin, nome);

    nome.append("@gmail.com"); // Adiciona uma string ao final
    std::cout << "Nome apos append(): '" << nome << "'" << std::endl;

    std::cout << "Escreva seu nome: ";
    std::getline(std::cin, nome);

    std::cout << nome.at(0) << std::endl; // Acessa o primeiro caractere da string
    std::cout << nome.at(1) << std::endl; // Acessa o segundo caractere da string
    std::cout << nome.at(2) << std::endl; // Acessa o terceiro caractere da string
    std::cout << nome.at(3) << std::endl; // Acessa o quarto caractere da string

    nome.insert(1, "@"); // Insere uma string na posição especificada
    std::cout << "Nome apos insert(): '" << nome << "'" << std::endl;

    std::cout << "Casa a qual a letra n aparece: " <<nome.find("n") << std::endl; // Encontra a posição do caractere desejado

    nome.erase(1, 1); // Remove caracteres a partir da posição especificada
    std::cout << "Nome apos erase(): '" << nome << "'" << std::endl;

    return 0;
}