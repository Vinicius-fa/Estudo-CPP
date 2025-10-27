#include <iostream>
#include <cmath>

int main() {
    double num1, num2;
    char operacao;

    std::cout << "Digite o primeiro numero a ser calculado: ";
    std::cin >> num1;

    std::cout << "Digite a operacao a ser realizada (+, -, /, *): ";
    std::cin >> operacao;

    std::cout << "Digite o segundo numero a ser calculado: ";
    std::cin >> num2;

    double resultado;

    if (operacao == "+") {
        resultado = num1 + num2;
        std::cout << "A soma dos numeros é : " << resultado << std::endl;
    } else if (operacao == "-") {
        resultado = num1 - num2;
        
    }

    return 0;
}