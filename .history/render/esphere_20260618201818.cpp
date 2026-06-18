#include <iostream>
#include <vector>
#include <cmath>
#include <thread>
#include <chrono>
#include <string>
#include <algorithm>
#include <conio.h>

#ifdef _WIN32
#include <windows.h>
#endif

const float PI = 3.14159265358979323846f;

const int SCREEN_WIDTH = 160;
const int SCREEN_HEIGHT = 55;

struct Vertex
{
    float x;
    float y;
    float z;
};

std::vector<Vertex> sphereMesh;

void buildSphereMesh()
{
    sphereMesh.clear();

    const float radius = 1.0f;

    const float thetaStep = 0.01f;
    const float phiStep = 0.005f;

    for (float theta = 0; theta < PI; theta += thetaStep)
    {
        float sinTheta = std::sin(theta);
        float cosTheta = std::cos(theta);

        for (float phi = 0; phi < 2.0f * PI; phi += phiStep)
        {
            float thetaDeg = theta * (180.0f / PI);
            float phiDeg = phi * (180.0f / PI);

            bool isGridLine =
                (static_cast<int>(thetaDeg) % 18 < 2) ||
                (static_cast<int>(phiDeg) % 28 < 2);

            if (!isGridLine)
                continue;

            float sinPhi = std::sin(phi);
            float cosPhi = std::cos(phi);

            sphereMesh.push_back({
                radius * sinTheta * cosPhi,
                radius * sinTheta * sinPhi,
                radius * cosTheta
            });
        }
    }
}

void renderSphere(
    float angleX,
    float angleY,
    float angleZ,
    float lightX,
    float lightY)
{
    std::vector<char> screen(SCREEN_WIDTH * SCREEN_HEIGHT, ' ');
    std::vector<float> zBuffer(SCREEN_WIDTH * SCREEN_HEIGHT, 0.0f);

    const float distance = 6.0f;
    const float k1 = SCREEN_HEIGHT * 2.2f;

    const std::string shades =
        " .'`^\",:;Il!i><~+_-?][}{1)(|\\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";

    float lightZ = -0.7f;

    float lightLen =
        std::sqrt(lightX * lightX +
                  lightY * lightY +
                  lightZ * lightZ);

    float lx = lightX / lightLen;
    float ly = lightY / lightLen;
    float lz = lightZ / lightLen;

    float cx = std::cos(angleX);
    float sx = std::sin(angleX);

    float cy = std::cos(angleY);
    float sy = std::sin(angleY);

    float cz = std::cos(angleZ);
    float sz = std::sin(angleZ);

    for (const auto& p : sphereMesh)
    {
        float x1 = p.x;
        float y1 = p.y * cx - p.z * sx;
        float z1 = p.y * sx + p.z * cx;

        float x2 = x1 * cy + z1 * sy;
        float y2 = y1;
        float z2 = -x1 * sy + z1 * cy;

        float x3 = x2 * cz - y2 * sz;
        float y3 = x2 * sz + y2 * cz;
        float z3 = z2;

        float nx = x3;
        float ny = y3;
        float nz = z3;

        float brightness =
            (std::max)(
                0.0f,
                nx * lx +
                ny * ly +
                nz * lz);

        float ooz = 1.0f / (z3 + distance);

        int xp = static_cast<int>(
            SCREEN_WIDTH / 2 +
            2.2f * k1 * ooz * x3);

        int yp = static_cast<int>(
            SCREEN_HEIGHT / 2 -
            k1 * ooz * y3);

        if (xp < 0 || xp >= SCREEN_WIDTH ||
            yp < 0 || yp >= SCREEN_HEIGHT)
        {
            continue;
        }

        int idx = yp * SCREEN_WIDTH + xp;

        if (ooz > zBuffer[idx])
        {
            zBuffer[idx] = ooz;

            int shadeIndex =
                static_cast<int>(
                    brightness *
                    (shades.size() - 1));

            shadeIndex =
                std::clamp(
                    shadeIndex,
                    0,
                    static_cast<int>(shades.size()) - 1);

            screen[idx] = shades[shadeIndex];
        }
    }

    std::string output;
    output.reserve(
        SCREEN_WIDTH * SCREEN_HEIGHT +
        SCREEN_HEIGHT + 512);

    output += "\x1b[H";

    for (int y = 0; y < SCREEN_HEIGHT; y++)
    {
        for (int x = 0; x < SCREEN_WIDTH; x++)
        {
            output += screen[y * SCREEN_WIDTH + x];
        }

        output += '\n';
    }

    output += "\n";
    output += "WASD = mover luz\n";
    output += "Luz X: " + std::to_string(lightX);
    output += " | Luz Y: " + std::to_string(lightY);
    output += "\n";

    std::cout << output << std::flush;
}

int main()
{
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    if (hOut != INVALID_HANDLE_VALUE)
    {
        DWORD mode = 0;

        if (GetConsoleMode(hOut, &mode))
        {
            mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hOut, mode);
        }
    }
#endif

    buildSphereMesh();

    std::cout << "\x1b[2J";

    float angleX = 0.0f;
    float angleY = 0.0f;
    float angleZ = 0.0f;

    float lightX = 0.5f;
    float lightY = 1.0f;

    while (true)
    {
        if (_kbhit())
        {
            char key = _getch();

            switch (key)
            {
            case 'w':
            case 'W':
                lightY += 0.2f;
                break;

            case 's':
            case 'S':
                lightY -= 0.2f;
                break;

            case 'a':
            case 'A':
                lightX -= 0.2f;
                break;

            case 'd':
            case 'D':
                lightX += 0.2f;
                break;
            }

            lightX = std::clamp(lightX, -3.0f, 3.0f);
            lightY = std::clamp(lightY, -3.0f, 3.0f);
        }

        renderSphere(
            angleX,
            angleY,
            angleZ,
            lightX,
            lightY);

        angleY += 0.010f;
        angleX = std::sin(angleY * 0.30f) * 0.40f;
        angleZ = std::cos(angleY * 0.20f) * 0.30f;

        std::this_thread::sleep_for(
            std::chrono::milliseconds(16));
    }

    return 0;
}