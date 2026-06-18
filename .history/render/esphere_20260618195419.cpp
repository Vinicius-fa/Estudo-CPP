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

void renderSphere(float angleX, float angleY) {
    std::vector<char> screen(SCREEN_WIDTH * SCREEN_HEIGHT, ' ');
    std::vector<float> zBuffer(SCREEN_WIDTH * SCREEN_HEIGHT, 0.0f);

    float radius = 1.0f;
    float distance = 5.0f;
    float k1 = SCREEN_HEIGHT * 2.0f;

    // Paleta de iluminação mais suave e espaçada
    const std::string illuminationChars = " .:-=+*#%@";

    // Luz vindo de cima, da direita e de trás da câmera
    float lightX = 0.5f;
    float lightY = 1.0f;
    float lightZ = -0.7f;
    float lightLen = std::sqrt(lightX * lightX + lightY * lightY + lightZ * lightZ);
    lightX /= lightLen; lightY /= lightLen; lightZ /= lightLen;

    // Passos mais finos para a esfera ficar mais definida
    float thetaStep = 0.02f;
    float phiStep = 0.01f;

    for (float theta = 0; theta < PI; theta += thetaStep) {
        float sinTheta = std::sin(theta);
        float cosTheta = std::cos(theta);

        for (float phi = 0; phi < 2 * PI; phi += phiStep) {
            
            // --- TRUQUE DA TEXTURA (MERIDIANOS E PARALELOS) ---
            // Só desenha o caractere se ele estiver perto de "linhas de grade" matemáticas
            // Isso cria o efeito de "globo terrestre aramado", permitindo ver a rotação nitidamente.
            float thetaDeg = theta * (180.0f / PI);
            float phiDeg = phi * (180.0f / PI);
            
            bool isGridLine = (static_cast<int>(thetaDeg) % 20 < 2) || 
                              (static_cast<int>(phiDeg) % 30 < 2);
            
            if (!isGridLine) continue; 

            float sinPhi = std::sin(phi);
            float cosPhi = std::cos(phi);

            // Posição 3D original
            float x0 = radius * sinTheta * cosPhi;
            float y0 = radius * sinTheta * sinPhi;
            float z0 = radius * cosTheta;

            // Vetor Normal
            float nx = sinTheta * cosPhi;
            float ny = sinTheta * sinPhi;
            float nz = cosTheta;

            // Rotação no eixo X
            float cosX = std::cos(angleX), sinX = std::sin(angleX);
            float y1 = y0 * cosX - z0 * sinX;
            float z1 = y0 * sinX + z0 * cosX;
            float ny1 = ny * cosX - nz * sinX;
            float nz1 = ny * sinX + nz * cosX;

            // Rotação no eixo Y
            float cosY = std::cos(angleY), sinY = std::sin(angleY);
            float x2 = x0 * cosY + z1 * sinY;
            float z2 = -x0 * sinY + z1 * cosY;
            float nx2 = nx * cosY + nz1 * sinY;
            float nz2 = -nx * sinY + nz1 * cosY;

            float ooZ = 1.0f / (z2 + distance); 

            // Projeção na tela (ajuste de proporção 2.2f para corrigir distorção do terminal)
            int xp = static_cast<int>(SCREEN_WIDTH / 2 + 2.2f * k1 * ooZ * x2);
            int yp = static_cast<int>(SCREEN_HEIGHT / 2 - k1 * ooZ * y1);

            // Produto escalar para iluminação (Brilho vai de -1 a 1)
            float brightness = nx2 * lightX + ny1 * lightY + nz2 * lightZ;

            if (xp >= 0 && xp < SCREEN_WIDTH && yp >= 0 && yp < SCREEN_HEIGHT) {
                if (ooZ > zBuffer[yp * SCREEN_WIDTH + xp]) {
                    zBuffer[yp * SCREEN_WIDTH + xp] = ooZ;
                    
                    // Se o brilho for muito baixo (parte de trás/sombra), damos um caractere padrão fraco
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

    while (true) {
        renderSphere(angleX, angleY);

        // Velocidades diferentes mudam drasticamente a percepção de direção!
        angleX += 0.01f; // Gira devagar verticalmente
        angleY += 0.04f; // Gira mais rápido horizontalmente (como a Terra)

        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }

    return 0;
}