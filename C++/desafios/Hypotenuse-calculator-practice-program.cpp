// Calculando a hipotenusa de um triangulo retangulo

#include <iostream>
#include <cmath>

int main() {

    double a;
    double b;
    double c;

    std::cout << "Digite o lado A: ";
    std::cin >> a;
    
    std::cout << "Digite o lado B: ";
    std::cin >> b;

    c = sqrt(pow(a, 2) + pow(b, 2));
    std::cout << "A hipotenusa é: " << c << std::endl;
    
    return 0;
}