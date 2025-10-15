#include <iostream>

/*
if = usado para executar um bloco de código se uma condição for verdadeira. 
se não for verdadeira, não sera executada, o bloco de código sera pulado.
Nele, usaremos muitos operadores de comparação, como:
> : maior que...
< : menor que...
== : igual que...
>= : maior ou igual que...
<= : menor ou igual que...
*/

int main() {

    int idade;

    std::cout << "Digite sua idade: ";
    std::cin >> idade;

    if (idade >= 18) {
        std::cout << "Vai ter que pagar conta kkkkk" << std::endl; 
    } else if (idade >= 100) { // Usado caso a condição anterior for falsa e executa outro bloco de código
        std::cout << "Se usa bengala kkkkkkkkkkkkkk" << std::endl;
    } else if (idade <= 0) {
        std::cout << "Você nem nasceu ainda zé kkkk" << std::endl; 
    } else { //Usado caso todas as condições anteriores forem falsas
        std::cout << "Idade inválida" << std::endl;
    }
    
    return 0;
}