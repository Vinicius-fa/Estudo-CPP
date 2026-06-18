#include <iostream>
#include <vector>
#include <cmath>
#include <thread>
#include <chrono>
#include <conio.h>

#ifdef _WIN32
#include <windows.h>
#endif

const float PI = 3.14159265358979323846f;

const int SCREEN_WIDTH = 160;   
const int SCREEN_HEIGHT = 55;   

void renderSphere(float angleX, float angleY, float angleZ, float lightX, float lightY) {
    std::vector<char> screen(SCREEN_WIDTH * SCREEN_HEIGHT, ' ');
    std::vector<float> zBuffer(SCREEN_WIDTH * SCREEN_HEIGHT, 0.0f);

    float radius = 1.0f;
    float distance = 5.0f;
    float k1 = SCREEN_HEIGHT * 2.2f; 

    const std::string illuminationChars = " .:-=+*#%@";
    float lightZ = -0.7f; 
    
    float lightLen = std::sqrt(lightX * lightX + lightY * lightY + lightZ * lightZ);
    float nxLight = lightX / lightLen;
    float nyLight = lightY / lightLen;
    float nzLight = lightZ / lightLen;

    // --- PASSOS MAIS FINOS PARA MOVIMENTO LISO ---
    // Diminuímos os passos para renderizar mais pontos, eliminando o aspecto serrilhado
    float thetaStep = 0.01f;
    float phiStep = 0.005f;

    float cx = std::cos(angleX), sx = std::sin(angleX);
    float cy = std::cos(angleY), sy = std::sin(angleY);
    float cz = std::cos(angleZ), sz = std::sin(angleZ);

    for (float theta = 0; theta < PI; theta += thetaStep) {
        float sinTheta = std::sin(theta);
        float cosTheta = std::cos(theta);

        for (float phi = 0; phi < 2 * PI; phi += phiStep) {
            
            float thetaDeg = theta * (180.0f / PI);
            float phiDeg = phi * (180.0f / PI);
            
            // Linhas de grade ligeiramente mais espaçadas para contraste nítido
            bool isGridLine = (static_cast<int>(thetaDeg) % 18 < 2) || 
                              (static_cast<int>(phiDeg) % 28 < 2);
            
            if (!isGridLine) continue; 

            float sinPhi = std::sin(phi);
            float cosPhi = std::cos(phi);

            float x0 = radius * sinTheta * cosPhi;
            float y0 = radius * sinTheta * sinPhi;
            float z0 = radius * cosTheta;

            // Rotações
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
            if (key == 'w' || key == 'W') lightY += 0.2f;
            if (key == 's' || key == 'S') lightY -= 0.2f;
            if (key == 'a' || key == 'A') lightX -= 0.2f;
            if (key == 'd' || key == 'D') lightX += 0.2f;
        }

        renderSphere(angleX, angleY, angleZ, lightX, lightY);

        // --- VELOCIDADE DE ROTAÇÃO REDUZIDA ---
        // Ângulos menores por frame fazem a transição de imagem parecer "fílmica" e suave
        angleX += 0.003f; 
        angleY += 0.009f; 
        angleZ += 0.006f; 

        // --- FRAME RATE OTIMIZADO ---
        // Reduzido para 16 milissegundos (~60 quadros por segundo)
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    return 0;
}