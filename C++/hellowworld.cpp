#include <iostream>

int main() {

    std::cout << "Hellow World!" << std::endl;
    std::cout << '\n';
    //isso é um comentário

    /*
    isto é 
    um comentário de 
    múltiplas linhas
    */

    // Explicação do código
    
    /*
    um comentário é uma anotação no código que não é executada, serve apenas para explicar o código para outros programadores ou para você mesmo no futuro
    #include = diretiva de pré-processador que inclui o conteúdo de um arquivo de cabeçalho
    <iostream> = biblioteca padrão de entrada e saída em C++
    std = standard
    cout = character output
    << = output operator
    >> = input operator
    endl = end line
    return 0 = encerra o programa com sucesso, se aparecer 1 ou outro número é porque houve algum erro
    */

    // tipos de dados em C++

    /*
    integer ou int = números inteiros (sem casas decimais)
    float = números com casas decimais (ponto flutuante)
    double = números com mais casas decimais (ponto flutuante de dupla precisão)
    char = caractere (letra, número ou símbolo)
    string = cadeia de caracteres (texto)
    bool = booleano (verdadeiro ou falso)
    */

    int x; // declaração de variável
    x = 5; // atribuição de valor à variável
    int y = 10; // declaração e atribuição em uma única linha
    int sum = x + y; // soma de duas variáveis

    
    std::cout << "O valor de x é: " << x << std::endl;
    std::cout << "O valor de y é: " << y << std::endl;
    std::cout << "A soma de x e y é: " << sum << std::endl;
    std::cout << '\n';

    std::string nome = "João"; // declaração e atribuição de string
    std::string sobrenome = "Silva";
    std::string nomeCompleto = nome + " " + sobrenome; // concatenação
    std::cout << "O meu nome completo é: " << nomeCompleto << " e eu tenho " << sum << " anos." << std::endl;
    return 0;
}