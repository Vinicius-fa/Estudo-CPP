#include <iostream>

/*
TERNARY OPERATOR EM CPP
O operador ternário em C++ é uma forma concisa de expressar uma condição if-else em uma única linha de código. Ele é composto por três partes:
1. A condição a ser avaliada.
2. O valor ou expressão a ser retornado se a condição for verdadeira.
3. O valor ou expressão a ser retornado se a condição for falsa.
*/

int main(){

    int nota = 75;

    nota >= 60 ? std::cout << "Aprovado!" << std::endl : std::cout << "Reprovado" << std::endl;

    bool comFome = true;
    
    comFome ? std::cout << "Noguete" << std::endl : std::cout << "Sem noguete pra voce" << std::endl; //Note como não é usad o "comFome == true", pois a variável já é booleana

    //também podemos usar o operador dentro de uma impressão

    comFome = false;

    std::cout << (comFome ? "Vou comer" : "Nao vou comer") << std::endl;


    return 0;
}