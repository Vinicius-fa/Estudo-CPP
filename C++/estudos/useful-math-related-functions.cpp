/*
    Useful math related functions ou Funções matemáticas úteis é um conjunto de funções matemáticas
    que podem ser usadas em vários programas.
    Muitas dessas funções são usadas em problemas de programação competitiva e podem ser úteis para 
    resolver problemas matemáticos comuns.
    Elas incluem funções para calcular o máximo divisor comum (MDC), mínimo múltiplo comum(MMC),
    verificar se um número é primo, calcular a potência de um número, entre outras.
*/

#include <iostream>
#include <cmath> //cmath é uma biblioteca que contém funções matemáticas comuns

int main () {

    double pi = 3.14;
    double x = 3;
    double y = 4;
    double z;

    z = std::max(x, y); //max retorna o maior valor entre x e y
    z = std::min(x, y); //min retorna o menor valor entre x e y
    z = std::pow(2, 4); //pow retorna o valor de 2 elevado a 4
    z = std::sqrt(16); //sqrt retorna a raiz quadrada de 16
    z = std::abs(-5); //abs retorna o valor absoluto de -5
    z = round(pi); //round arredonda o valor de pi para o inteiro mais próximo
    z = ceil(pi); //ceil arredonda o valor de pi para cima
    z = floor(pi); //floor arredonda o valor de pi para baixo

    return 0;
}