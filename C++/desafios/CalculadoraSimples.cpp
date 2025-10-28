#include <iostream>
#include <cmath>

int main() {
    double num1, num2;
    char operacao;

    std::cout << "Digite o primeiro numero a ser calculado: " << std::endl;
    std::cin >> num1;

    std::cout << "Digite a operacao a ser realizada (+, -, /, *): " << std::endl;
    std::cin >> operacao;

    std::cout << "Digite o segundo numero a ser calculado: " << std::endl;
    std::cin >> num2;

    double resultado;

    switch (operacao) {
        case '+':
            resultado = num1 + num2;
            break;
        case '-':
            resultado = num1 - num2;
            break;
        case '*':
            resultado = num1 * num2;
            break;
        case '/':
            if (num2 != 0) {
                resultado = num1 / num2;
            } else {
                std::cout << "Erro: Divisao por zero nao e permitida." << std::endl;
                return 1; // Sai do programa com codigo de erro
            }
            
            break;

        default:
            std::cout << "Operacao invalida. Por favor, use +, -, * ou /." << std::endl;
            return 1; // Sai do programa com codigo de erro
    }

    /*nota:
    É importante notar o uso de aspas unicas ao inves de duplas para a variavel do tipo char.
    Isso porque aspas duplas são usadas para strings (conjuntos de caracteres),
    enquanto aspas simples são usadas para caracteres individuais.
    */

    return 0;
}