#include <iostream>
#include <vector>
#include <cmath>
#include <thread>
#include <chrono>

// Constantes matemáticas básicas
const float PI = 3.14159265358979323846f;

// Tamanho da tela do terminal
const int SCREEN_WIDTH = 80;
const int SCREEN_HEIGHT = 40;

void renderSphere(float angleX, float angleY) {
    // Inicializa a tela com espaços em branco e o buffer de profundidade (z-buffer)
    std::vector<char> screen(SCREEN_WIDTH * SCREEN_HEIGHT, ' ');
    std::vector<float> zBuffer(SCREEN_WIDTH * SCREEN_HEIGHT, 0.0f);

    // Configurações da esfera e da câmera
    float radius = 1.5f;          // Raio da esfera no espaço 3D
    float distance = 5.0f;        // Distância da câmera até a esfera
    float k1 = SCREEN_HEIGHT * 2.0f; // Fator de escala para projeção na tela

    // Paleta de iluminação ASCII (do mais escuro para o mais claro)
    const std::string illuminationChars = ".,-~:;=!*#$@";

    // Direção de uma luz direcional fictícia (vetor normalizado)
    float lightX = 0.0f;
    float lightY = 1.0f;
    float lightZ = -1.0f;
    float lightLen = std::sqrt(lightX * lightX + lightY * lightY + lightZ * lightZ);
    lightX /= lightLen; lightY /= lightLen; lightZ /= lightLen;

    // Passos para percorrer a superfície da esfera (ângulos teta e phi)
    float thetaStep = 0.05f;
    float phiStep = 0.02f;

    for (float theta = 0; theta < PI; theta += thetaStep) {
        float sinTheta = std::sin(theta);
        float cosTheta = std::cos(theta);

        for (float phi = 0; phi < 2 * PI; phi += phiStep) {
            float sinPhi = std::sin(phi);
            float cosPhi = std::cos(phi);

            // 1. Coordenadas esféricas originais da superfície
            float x0 = radius * sinTheta * cosPhi;
            float y0 = radius * sinTheta * sinPhi;
            float z0 = radius * cosTheta;

            // Como é uma esfera centrada na origem, o vetor normal (direção da superfície) 
            // é igual à própria posição do ponto
            float nx = sinTheta * cosPhi;
            float ny = sinTheta * sinPhi;
            float nz = cosTheta;

            // 2. Aplicar rotação no eixo X (angleX)
            float cosX = std::cos(angleX), sinX = std::sin(angleX);
            float y1 = y0 * cosX - z0 * sinX;
            float z1 = y0 * sinX + z0 * cosX;
            
            float ny1 = ny * cosX - nz * sinX;
            float nz1 = ny * sinX + nz * cosX;

            // 3. Aplicar rotação no eixo Y (angleY)
            float cosY = std::cos(angleY), sinY = std::sin(angleY);
            float x2 = x0 * cosY + z1 * sinY;
            float z2 = -x0 * sinY + z1 * cosY;

            float nx2 = nx * cosY + nz1 * sinY;
            float nz2 = -nx * sinY + nz1 * cosY;
            float ny2 = ny1; // Não muda na rotação Y

            // Deslocar a esfera para frente da câmera no eixo Z
            float ooZ = 1.0f / (z2 + distance); 

            // 4. Projeção Perspectiva 3D para 2D (ajustando a proporção do caractere do terminal)
            // Caracteres de terminal geralmente são mais altos do que largos, multiplicamos o X por 2 para corrigir
            int xp = static_cast<int>(SCREEN_WIDTH / 2 + 2.0f * k1 * ooZ * x2);
            int yp = static_cast<int>(SCREEN_HEIGHT / 2 - k1 * ooZ * y1); // y1 invertido para coordenadas de tela

            // 5. Cálculo de iluminação (Produto escalar entre a Normal e a Luz)
            float brightness = nx2 * lightX + ny2 * lightY + nz2 * lightZ;

            // Se o ponto estiver dentro da tela e for o ponto mais próximo da câmera (Z-Buffer)
            if (xp >= 0 && xp < SCREEN_WIDTH && yp >= 0 && yp < SCREEN_HEIGHT) {
                if (ooZ > zBuffer[yp * SCREEN_WIDTH + xp]) {
                    zBuffer[yp * SCREEN_WIDTH + xp] = ooZ;
                    
                    // Converte o brilho para um índice da nossa string de caracteres
                    int charIndex = static_cast<int>((brightness + 1.0f) / 2.0f * (illuminationChars.size() - 1));
                    if (charIndex < 0) charIndex = 0;
                    if (charIndex >= illuminationChars.size()) charIndex = illuminationChars.size() - 1;

                    screen[yp * SCREEN_WIDTH + xp] = illuminationChars[charIndex];
                }
            }
        }
    }

    // Limpa a tela movendo o cursor para o topo (evita o piscar de tela comum)
    std::cout << "\x1b[H";
    
    // Desenha a matriz na tela
    std::string output = "";
    for (int y = 0; y < SCREEN_HEIGHT; ++y) {
        for (int x = 0; x < SCREEN_WIDTH; ++x) {
            output += screen[y * SCREEN_WIDTH + x];
        }
        output += '\n';
    }
    std::cout << output;
}

int main() {
    // Limpa a tela do terminal antes de começar
    std::cout << "\x1b[2J";

    float angleX = 0.0f;
    float angleY = 0.0f;

    // Loop principal de animação
    while (true) {
        renderSphere(angleX, angleY);

        // Incrementa os ângulos para fazer a esfera girar
        angleX += 0.03f;
        angleY += 0.02f;

        // Controla a taxa de quadros (aprox. 30 FPS)
        std::this_thread::sleep_for(std::chrono::milliseconds(33));
    }

    return 0;
}