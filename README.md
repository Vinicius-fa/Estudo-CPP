# Estudo-CPP

Uma coleção pessoal de exercícios e pequenos projetos em C++ para registrar meu aprendizado e compartilhar com quem quiser acompanhar. Toda ajuda é bem-vinda — sinta-se à vontade para ver, comentar, abrir issues ou enviar PRs.

## Sumário
- [Sobre](#sobre)
- [Requisitos](#requisitos)
- [Como compilar](#como-compilar)
- [Como executar](#como-executar)
- [Estrutura sugerida](#estrutura-sugerida)
- [Contribuição](#contribuição)
- [Recursos](#recursos)
- [Contato](#contato)

## Sobre
Este repositório reúne exercícios, exemplos e pequenos projetos em C++ usados durante meus estudos.

## Requisitos
- g++ (recomendo usar -std=c++17)
- make (opcional)
- Sistema Unix-like ou Windows (com ambiente compatível)

## Como compilar
Exemplos rápidos sem CMake:

- Compilar um único arquivo:
```bash
g++ -std=c++17 -O2 -Wall src/exemplo.cpp -o bin/exemplo
```

- Compilar múltiplos arquivos:
```bash
g++ -std=c++17 -O2 -Wall src/main.cpp src/foo.cpp src/bar.cpp -o bin/app
```

- Criar pastas de bin se necessário:
```bash
mkdir -p bin
```

Ajuste nomes e caminhos conforme cada exercício/pasta.

## Como executar
Após compilar:
```bash
./bin/exemplo
# ou
./bin/app arg1 arg2
```

## Estrutura sugerida
- src/ — código-fonte
- include/ — headers (.h/.hpp)
- examples/ — exercícios individuais (cada pasta com README)
- bin/ — binários gerados
- tests/ — testes (opcional)

## Contribuição
Quer ver algo diferente ou colaborar? Abra uma issue ou um pull request. Toda ajuda é bem-vinda: correções, sugestões de exercícios, melhorias de README por pasta, exemplos de entrada/saída, etc.

Se for enviar PR:
1. Faça um fork.
2. Crie uma branch descritiva (ex.: feat/exercicio-nome).
3. Adicione um README na pasta do exercício explicando como compilar/rodar.
4. Abra o PR descrevendo as mudanças.

## Recursos
- Canal Bro Code no YouTube: https://www.youtube.com/@BroCode  
- Curso completo de C++ (pesquisa do curso do Bro Code): https://www.youtube.com/results?search_query=Bro+Code+C%2B%2B+full+course

## Contato
GitHub: https://github.com/Vinicius-fa
