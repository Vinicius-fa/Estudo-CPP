#include <iostream>

int main() {

    double temperatura;
    char unidade;

    std::cout << "==============CONVERSOR DE TEMPERATURA==============\n";
    std::cout << "Digite a medida que deseja descobrir a temperatura" << std::endl;
    std::cout << "C = celcius " << std::endl;
    std::cout << "F = fahrenheit " << std::endl;
    std::cout << ": ";
    std::cin >> unidade;

    if (unidade == 'F' || unidade == 'f') {
        std::cout << "Digite a temperatura em celcius: " << std::endl;
        std::cin >> temperatura;

        temperatura = (1,8 *temperatura) + 32;
        std::cout << "A temperatura em fahrenheit é: " << temperatura << " F" << std::endl;

    } else if (unidade == 'C' || unidade == 'c') {
        std::cout << "Digite a temperatura em fahrenheit: " << std::endl;
        std::cin >> temperatura;

        temperatura = (temperatura - 32) / 1.8;
        std::cout << "A temperatura em celcius é: " << temperatura << " C" << std::endl;

    } else {
        std::cout << "Unidade inválida! Por favor, use 'C' para celcius ou 'F' para fahrenheit." << std::endl;

    }

    return 0;
}