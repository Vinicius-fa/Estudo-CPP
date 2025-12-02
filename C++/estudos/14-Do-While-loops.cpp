#include<iostream>
#include<string>

int main(){

    //do while = Executa um bloco de código, e DEPOIS verifica a condição. Ou seja, o bloco de código sempre será executado ao menos 1 vez.

    int numero;

    do {                                            //Do = Faça
        std::cout << "Digite um numero positivo: ";
        std::cin >> numero;
    } while(numero < 0);                           //While = Enquanto

    std::cout << "O numero digitado foi: " << numero;

    return 0;
}