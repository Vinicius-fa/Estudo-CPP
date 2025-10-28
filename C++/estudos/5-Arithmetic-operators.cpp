/*
Arithmetic-operators ou operadores aritméticos são símbolos que representam operações matemáticas básicas em C++.
Eles são usados para realizar cálculos com variáveis e valores numéricos.
Os principais operadores aritméticos em C++ são:
+ (adição): soma dois valores.
- (subtração): subtrai o segundo valor do primeiro.
* (multiplicação): multiplica dois valores.
/ (divisão): divide o primeiro valor pelo segundo.
% (módulo): retorna o resto da divisão do primeiro valor pelo segundo (apenas para inteiros).
++ (incremento): aumenta o valor de uma variável em 1.
-- (decremento): diminui o valor de uma variável em 1.
Esses operadores podem ser usados em expressões matemáticas para calcular resultados, atribuir valores a variáveis e manipular dados numéricos.
*/

#include <iostream>

int main() {

    int a = 10;
    int b = 3;

    std::cout << "a = " << a << ", b = " << b << std::endl;
    std::cout << "Soma: a + b = " << a + b << std::endl; // adição
    std::cout << "Subtração: a - b = " << a - b << std::endl; // subtração
    std::cout << "Multiplicação: a * b = " << a * b << std::endl; // multiplicação
    std::cout << "Divisão: a / b = " << a / b << std::endl; // divisão
    std::cout << "Módulo: a % b = " << a % b << std::endl; // módulo
    std::cout << "Incremento: ++a = " << ++a << std::endl; // incremento
    std::cout << "Decremento: --b = " << --b << std::endl; // decremento
    std::cout << "Valor final de a: " << a << std::endl; // valor final de a
    std::cout << "Valor final de b: " << b << std::endl; // valor final de b

    return 0;
}