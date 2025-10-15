/*
    const ou constante é um valor que não pode ser alterado durante a execução do programa, um "read only".
    Usado para valores que não devem ser modificados, como pi, número máximo de tentativas, etc.
    Ajuda a evitar erros acidentais e torna o código mais fácil de entender.
    Uma boa prática é usar letras maiúsculas para nomes de constantes, por exemplo: const int MAX_TENTATIVAS = 5;
*/

#include <iostream>

int main() {
    
    const int IDADE = 25; // declaração e atribuição de constante
    std::cout << "A minha idade é: " << IDADE << std::endl;
    //idade = 30; erro! não é possível alterar o valor de uma constante
    

    return 0;
}