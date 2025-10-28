#include <iostream>

/*
        SWITCHES EM CPP
    Switches são estruturas de controle de fluxo que permitem executar diferentes partes do código com base no valor de uma variável ou expressão.
    Eles são úteis quando você tem várias condições para verificar e deseja evitar o uso excessivo de declarações if-else.
    A sintaxe básica de um switch em C++ é a seguinte:
*/
int main() {

    int opcao;
    std::cout << "Escolha uma opcao (1-5): ";
    std::cin >> opcao;

    switch (opcao) { // A palavra-chave 'switch' é seguida pela variável ou expressão a ser avaliada entre parênteses
        case 1: // Cada 'case' representa um valor possível da variável ou expressão, como no caso: 1
            std::cout << "Voce escolheu a opcao 1." << std::endl;
            break; // O break é usado para sair do switch após a execução do caso correspondente
        case 2:
            std::cout << "Voce escolheu a opcao 2." << std::endl;
            break;
        case 3:
            std::cout << "Voce escolheu a opcao 3." << std::endl;
            break;
        case 4:
            std::cout << "Voce escolheu a opcao 4." << std::endl;
            break;
        case 5:
            std::cout << "Voce escolheu a opcao 5." << std::endl;
            break;
        default:
            std::cout << "Opcao invalida. Por favor, escolha um numero entre 1 e 5." << std::endl;
            break;
    }

    return 0;
}