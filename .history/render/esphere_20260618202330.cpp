#include <iostream>
#include <vector>
#include <cmath>
#include <thread>
#include <chrono>
#include <sstream>
#include <conio.h>

#ifdef _WIN32
#include <windows.h>
#endif

const float PI = 3.14159265358979323846f;

// --- RESOLUÇÃO EXPANDIDA PARA TERMINALS EM TELA CHEIA ---
const int SCREEN_WIDTH = 160;   // Aumentamos a largura para preencher a tela
const int SCREEN_HEIGHT = 55;   // Aumentamos a altura proporcionalmente

void renderSphere(float angleX, float angleY, float angleZ, float lightX, float lightY) {
    std::vector<char> screen(SCREEN_WIDTH * SCREEN_HEIGHT, ' ');
    std::vector<float> zBuffer(SCREEN_WIDTH * SCREEN_HEIGHT, 0.0f);

    // Ajuste do tamanho e distância para a nova resolução
    float radius = 1.0f;
    float distance = 6.0f;
    
    // k1 reajustado para a nova altura da tela
    float k1 = SCREEN_HEIGHT * 2.2f; 

    const std::string illuminationChars = " .:-=+*#%@";
    float lightZ = -0.7f; 
    
    float lightLen = std::sqrt(lightX * lightX + lightY * lightY + lightZ * lightZ);
    float nxLight = lightX / lightLen;
    float nyLight = lightY / lightLen;
    float nzLight = lightZ / lightLen;

    // Passos ligeiramente menores para não deixar buracos na esfera maior
    float thetaStep = 0.015f;
    float phiStep = 0.008f;

    float cx = std::cos(angleX), sx = std::sin(angleX);
    float cy = std::cos(angleY), sy = std::sin(angleY);
    float cz = std::cos(angleZ), sz = std::sin(angleZ);

    for (float theta = 0; theta < PI; theta += thetaStep) {
        float sinTheta = std::sin(theta);
        float cosTheta = std::cos(theta);

        for (float phi = 0; phi < 2 * PI; phi += phiStep) {
            
            float thetaDeg = theta * (180.0f / PI);
            float phiDeg = phi * (180.0f / PI);
            
            // Ajustamos a densidade das linhas de grade para a nova escala
            bool isGridLine = (static_cast<int>(thetaDeg) % 15 < 2) || 
                              (static_cast<int>(phiDeg) % 25 < 2);
            
            if (!isGridLine) continue; 

            float sinPhi = std::sin(phi);
            float cosPhi = std::cos(phi);

            float x0 = radius * sinTheta * cosPhi;
            float y0 = radius * sinTheta * sinPhi;
            float z0 = radius * cosTheta;

            // Rotações nos eixos X, Y e Z
            float x1 = x0;
            float y1 = y0 * cx - z0 * sx;
            float z1 = y0 * sx + z0 * cx;

            float x2 = x1 * cy + z1 * sy;
            float y2 = y1;
            float z2 = -x1 * sy + z1 * cy;

            float x3 = x2 * cz - y2 * sz;
            float y3 = x2 * sz + y2 * cz;
            float z3 = z2;

            float nx = x3 / radius;
            float ny = y3 / radius;
            float nz = z3 / radius;

            float ooZ = 1.0f / (z3 + distance); 

            // --- CENTRALIZAÇÃO CORRIGIDA ---
            // O fator 2.2f ajusta o aspecto da fonte do Windows Terminal para mantê-la esférica
            int xp = static_cast<int>((SCREEN_WIDTH / 2) + 2.2f * k1 * ooZ * x3);
            int yp = static_cast<int>((SCREEN_HEIGHT / 2) - k1 * ooZ * y3);

            float brightness = nx * nxLight + ny * nyLight + nz * nzLight;

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

    std::cout << "\x1b[H";
    std::string output = "";
    for (int y = 0; y < SCREEN_HEIGHT; ++y) {
        for (int x = 0; x < SCREEN_WIDTH; ++x) {
            output += screen[y * SCREEN_WIDTH + x];
        }
        output += '\n';
    }
    
    output += " CONTROLES DA LUZ (WASD) -> X: " + std::to_string(lightX) + " | Y: " + std::to_string(lightY) + "      \n";
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

    float angleX = 0.0f, angleY = 0.0f, angleZ = 0.0f;
    float lightX = 0.5f;
    float lightY = 1.0f;

    while (true) {
        if (_kbhit()) {
            char key = _getch();
            
            // Altera a posição com os limites (clamping) usando std::fmax e std::fmin
            if (key == 'w' || key == 'W') lightY = std::fmin(lightY + 0.2f, 3.0f);   // Trava no topo em 3.0
            if (key == 's' || key == 'S') lightY = std::fmax(lightY - 0.2f, -3.0f);  // Trava embaixo em -3.0
            if (key == 'a' || key == 'A') lightX = std::fmax(lightX - 0.2f, -3.0f);  // Trava na esquerda em -3.0
            if (key == 'd' || key == 'D') lightX = std::fmin(lightX + 0.2f, 3.0f);   // Trava na direita em 3.0
        }

        renderSphere(angleX, angleY, angleZ, lightX, lightY);

        angleX += 0.01f;
        angleY += 0.03f;
        angleZ += 0.02f;

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    return 0;
}