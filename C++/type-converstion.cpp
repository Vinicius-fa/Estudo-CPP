/*
Type conversion ou conversão de tipo é o processo de transformar um valor de um tipo de dado em outro tipo de dado em C++.
Isso é útil quando você precisa realizar operações entre diferentes tipos de dados ou quando deseja garantir que os
valores sejam tratados de maneira adequada. Existem dois tipos principais de conversão de tipo em C++:
conversão implícita e conversão explícita (casting).
Conversão implícita ocorre automaticamente quando o compilador converte um tipo de dado para outro sem a necessidade
de intervenção do programador. Por exemplo, quando você atribui um valor inteiro a uma variável de ponto flutuante,
o compilador converte automaticamente o inteiro para ponto flutuante.

Conversão explícita, ou casting, é quando o programador especifica manualmente a conversão de um tipo de dado para outro.
Isso pode ser feito usando operadores de casting, como static_cast, dynamic_cast, const_cast e reinterpret_cast.
*/

#include <iostream>

int main(){

    //implicita
    int x = 3.14;
    std::cout << "Valor de x (int): " << x << std::endl; // x será 3, pois a parte decimal é truncada

    char y = 100;
    std::cout << "Valor de y (char): " << y << std::endl; // y será 'd', pois 100 é o código ASCII para 'd'

    //explicita
    double a = (int)9.99; //Colocamos o parenteses para indicar que queremos fazer o casting
    std::cout << "Valor de a (double): " << a << std::endl; // a será 9.0, pois o casting para int trunca a parte decimal

    std::cout << "estamos printando a letra: " << (char) 100 << std::endl; // como colocamos o "(char)" antes do 100, o valor será convertido para char, resultando em 'd'

    std::cout << "======================================================" << std::endl;
    std::cout << "================   EXEMPLO PRÁTICO   =================" << std::endl;
    std::cout << "======================================================" << std::endl;

    int correto = 8;
    int perguntas = 10;
    double pontuacao = correto/perguntas * 100; // aqui a divisão é feita entre dois inteiros, resultando em 0

    std::cout << "Pontuação sem casting: " << pontuacao << "%" << std::endl; // pontuacao será 0.0

    //Para arrumar, podemos fazer o casting de um dos operandos para double
    pontuacao = correto/(double)perguntas * 100; // agora a divisão é feita entre um inteiro e um double, resultando em 0.8
    std::cout << "Pontuação com casting: " << pontuacao << "%" << std::endl; // pontuacao será 80.0

    return 0;
}