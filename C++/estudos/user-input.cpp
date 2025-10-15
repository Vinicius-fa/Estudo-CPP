/*
user-input ou entrada do usuário refere-se ao processo de obter dados ou informações do usuário durante a execução de um programa em C++.
Isso é comumente feito usando a biblioteca padrão de entrada e saída (iostream) e o objeto std::cin para ler dados do teclado.
A entrada do usuário pode ser de vários tipos, como números inteiros, números de ponto flutuante, caracteres ou strings.
Para ler a entrada do usuário, você pode usar o operador de extração (>>) com std::cin, seguido pela variável onde deseja armazenar o valor lido.
É importante validar a entrada do usuário para garantir que os dados fornecidos sejam do tipo esperado e estejam dentro dos limites aceitáveis.
*/

#include <iostream>
#include <string>

int main(){

    //cout << (console output) é usado para exibir informações na tela
    //cin >> (console input) é usado para ler informações do teclado

    std::string nome;
    int idade;

    std::cout << "Digite seu nome: "; // prompt para o usuário
    std::cin >> nome; // lê o nome do usuário e armazena na variável 'nome'

    std::cout << "quantos anos voce tem? ";
    std::cin >> idade; // lê a idade do usuário e armazena na variável 'idade'

    std::cout << "Ola, " << nome << ", voce tem " << idade << " anos" << std::endl;

    std::cout << "Digite seu nome completo: ";
    std::getline(std::cin >> std::ws, nome); // lê uma linha completa, incluindo espaços, e armazena na variável 'nome'

    return 0;
}