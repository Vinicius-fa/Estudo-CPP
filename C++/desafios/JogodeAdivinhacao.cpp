//jogo de adivinhação em cpp

#include <iostream>
#include <cstdlib> // para funções de geração de números aleatórios
#include <ctime> // para inicializar o gerador de números aleatórios com o tempo atual

int main() {
    std::srand(std::time(0)); // inicializa o gerador de numeros aleatorios com a hora atual
    int numeroSecreto = std::rand() % 100 + 1; // gera um numero aleatorio entre 1 e 100
    int palpite;
    int tentativas = 0;

    std::cout << "Bem-vindo ao jogo de adivinhacao!" << std::endl;
    std::cout << "Tente adivinhar o numero secreto entre 1 e 100." << std::endl;

    do {
        std::cout << "Digite seu palpite: ";
        std::cin >> palpite;
        tentativas++;

        if (palpite < numeroSecreto) {
            std::cout << "Muito baixo! Tente novamente." << std::endl;
        } else if (palpite > numeroSecreto) {
            std::cout << "Muito alto! Tente novamente." << std::endl;
        } else {
            std::cout << "Parabens! Voce adivinhou o numero secreto em " << tentativas << " tentativas." << std::endl;
        }
    } while (palpite != numeroSecreto);

    return 0;
}