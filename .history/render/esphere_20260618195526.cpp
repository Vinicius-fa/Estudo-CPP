#include <iostream>
#include <vector>
#include <cmath>
#include <thread>
#include <chrono>

#ifdef _WIN32
#include <windows.h>
#endif

const float PI = 3.14159265358979323846f;
const int SCREEN_WIDTH = 80;
const int SCREEN_HEIGHT = 40;

// Função atualizada recebendo os três ângulos
void renderSphere(float angleX, float angleY, float angleZ) {
    std::vector<char> screen(SCREEN_WIDTH * SCREEN_HEIGHT, ' ');
    std::vector<float> zBuffer(SCREEN_WIDTH * SCREEN_HEIGHT, 0.0f);

    float radius = 1.0f;
    float distance = 6.0f;
    float k1 = SCREEN_HEIGHT * 2.0f;

    const std::string illuminationChars = " .:-=+*#%@";

    // Luz vindo levemente de cima, direita e frente
    float lightX = 0.5f;
    float lightY = 1.0f;
    float lightZ = -0.7f;
    float lightLen = std::sqrt(lightX * lightX + lightY * lightY + lightZ * lightZ);
    lightX /= lightLen; lightY /= lightLen; lightZ /= lightLen;

    float thetaStep = 0.02f;
    float phiStep = 0.01f;

    // Pré-calcula senos e cossenos dos ângulos de rotação para poupar processamento
    float cx = std::cos(angleX), sx = std::sin(angleX);
    float cy = std::cos(angleY), sy = std::sin(angleY);
    float cz = std::cos(angleZ), sz = std::sin(angleZ);

    for (float theta = 0; theta < PI; theta += thetaStep) {
        float sinTheta = std::sin(theta);
        float cosTheta = std::cos(theta);

        for (float phi = 0; phi < 2 * PI; phi += phiStep) {
            
            // Efeito de grade/meridianos para enxergar o giro
            float thetaDeg = theta * (180.0f / PI);
            float phiDeg = phi * (180.0f / PI);
            bool isGridLine = (static_cast<int>(thetaDeg) % 20 < 2) || 
                              (static_cast<int>(phiDeg) % 30 < 2);
            
            if (!isGridLine) continue; 

            float sinPhi = std::sin(phi);
            float cosPhi = std::cos(phi);

            // 1. Coordenadas esféricas originais
            float x0 = radius * sinTheta * cosPhi;
            float y0 = radius * sinTheta * sinPhi;
            float z0 = radius * cosTheta;

            // --- ROTAÇÃO 3D COMPLETA (X, Y e Z) ---
            
            // Rotação no eixo X
            float x1 = x0;
            float y1 = y0 * cx - z0 * sx;
            float z1 = y0 * sx + z0 * cx;

            // Rotação no eixo Y
            float x2 = x1 * cy + z1 * sy;
            float y2 = y1;
            float z2 = -x1 * sy + z1 * cy;

            // Rotação no eixo Z (A novidade aqui!)
            float x3 = x2 * cz - y2 * sz;
            float y3 = x2 * sz + y2 * cz;
            float z3 = z2;

            // Como é uma esfera, o vetor normal (direção do brilho) 
            // aponta exatamente na mesma direção dos pontos rotacionados!
            float nx = x3 / radius;
            float ny = y3 / radius;
            float nz = z3 / radius;

            // Distância invertida para o efeito de perspectiva (Z-Buffer)
            float ooZ = 1.0f / (z3 + distance); 

            // Projeção na tela 2D
            int xp = static_cast<int>(SCREEN_WIDTH / 2 + 2.2f * k1 * ooZ * x3);
            int yp = static_cast<int>(SCREEN_HEIGHT / 2 - k1 * ooZ * y3);

            // Cálculo do brilho (Produto escalar entre a Normal e a Luz)
            float brightness = nx * lightX + ny * lightY + nz * lightZ;

            if (xp >= 0 && xp < SCREEN_WIDTH && yp >= 0 && yp < SCREEN_HEIGHT) {
                if (ooZ > zBuffer[yp * SCREEN_WIDTH + xp]) {
                    zBuffer[yp * SCREEN_WIDTH + xp] = ooZ;
                    
                    int charIndex = static_cast<int>((brightness + 1.0f) / 2.0f * (illuminationChars.size() - 1));
                    if (charIndex < 0) charIndex = 0;
                    if (charIndex >= illuminationChars.size()) charIndex = illuminationChars.size() - 1;

                    screen[yp * SCREEN_WIDTH + xp] = illuminationChars[charIndex];
                }
            }
        }
    }

    // Desenha o frame no terminal
    std::cout << "\x1b[H";
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
    #ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE) {
        DWORD dwMode = 0;
        if (GetConsoleMode(hOut, &dwMode)) {
            dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hOut, dwMode);
        }
    }
    #endif

    std::cout << "\x1b[2J";

    float angleX = 0.0f;
    float angleY = 0.0f;
    float angleZ = 0.0f;

    while (true) {
        renderSphere(angleX, angleY, angleZ);

        // Incrementa os 3 eixos com velocidades diferentes para o efeito ficar dinâmico
        angleX += 0.01f; // Inclinação vertical suave
        angleY += 0.03f; // Rotação horizontal média
        angleZ += 0.02f; // Giro "capotando" para o lado

        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }

    return 0;
}