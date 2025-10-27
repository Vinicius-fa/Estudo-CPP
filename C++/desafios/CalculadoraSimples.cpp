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

    if (operacao == '+') {
        resultado = num1 + num2;
        std::cout << "A soma dos numeros é : " << resultado << std::endl;
    } else if (operacao == '-') {
        resultado = num1 - num2;
        std::cout << "A subtração dos numeros é: " << resultado << std::endl;
    } else if (operacao == '/') {
        resultado = num1 / num2;
        std::cout << "A divisão dos numeros é: " << resultado << std::endl;
    } else if (operacao == '*') {
        resultado = num1 * num2;
        std::cout << "A multiplicação dos numeros é: " << resultado << std::endl;
    } else {
        std::cout << "Dados invalidos inseridos" << std::endl;
    }

    /*nota:
    É importante notar o uso de aspas unicas ao inves de duplas para a variavel do tipo char.
    Isso porque aspas duplas são usadas para strings (conjuntos de caracteres),
    enquanto aspas simples são usadas para caracteres individuais.
    */

    return 0;
}