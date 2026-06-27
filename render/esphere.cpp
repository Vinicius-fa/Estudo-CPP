#include <iostream>
#include <vector>
#include <cmath>
#include <thread>
#include <chrono>
#include <string>
#include <algorithm>
#include <cstdio>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

constexpr float PI = 3.14159265358979323846f;

// ============================================================
// MATH
// ============================================================

struct Vec3 {
    float x = 0, y = 0, z = 0;
    Vec3() = default;
    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
    float dot(const Vec3& o)  const { return x*o.x + y*o.y + z*o.z; }
    float length()            const { return std::sqrt(x*x + y*y + z*z); }
    Vec3  normalized()        const {
        float l = length();
        return l > 1e-6f ? Vec3{x/l, y/l, z/l} : Vec3{};
    }
};

Vec3 rotateX(Vec3 p, float a) { float c=cos(a),s=sin(a); return {p.x, p.y*c-p.z*s, p.y*s+p.z*c}; }
Vec3 rotateY(Vec3 p, float a) { float c=cos(a),s=sin(a); return {p.x*c+p.z*s, p.y, -p.x*s+p.z*c}; }
Vec3 rotateZ(Vec3 p, float a) { float c=cos(a),s=sin(a); return {p.x*c-p.y*s, p.x*s+p.y*c, p.z}; }

Vec3 applyRot(Vec3 p, Vec3 rot) {
    return rotateZ(rotateY(rotateX(p, rot.x), rot.y), rot.z);
}

// ============================================================
// TIPOS DE CENA
// ============================================================

struct Light {
    Vec3 dir;
    float r, g, b, intensity;
};

struct Sample {
    Vec3 pos, normal;
    bool isGrid;
};

enum class GridMode  { LATLON, DIAGONAL, SOLID };
enum class Primitive { SPHERE, TORUS, CONE, CYLINDER };

const char* primNames[] = { "Esfera", "Torus",    "Cone",     "Cilindro" };
const char* gridNames[] = { "Lat/Lon", "Diagonal", "Solido" };

// ============================================================
// GERADORES DE AMOSTRAS
// ============================================================

static bool gridTest(GridMode m, float da, float db) {
    switch (m) {
        case GridMode::LATLON:
            return (int(da) % 15 < 2) || (int(db) % 25 < 2);
        case GridMode::DIAGONAL:
            return (int(da + db) % 30 < 2) || (int(std::fabs(da - db)) % 30 < 2);
        case GridMode::SOLID:
            return true;
    }
    return true;
}

std::vector<Sample> makeSphere(GridMode m) {
    std::vector<Sample> v;
    bool solid = (m == GridMode::SOLID);
    float ts = solid ? 0.030f : 0.012f;
    float ps = solid ? 0.015f : 0.006f;
    for (float th = 0; th < PI; th += ts) {
        float sth=sin(th), cth=cos(th), tdeg=th*(180/PI);
        for (float ph = 0; ph < 2*PI; ph += ps) {
            if (!gridTest(m, tdeg, ph*(180/PI))) continue;
            float x=sth*cos(ph), y=sth*sin(ph), z=cth;
            v.push_back({ {x,y,z}, {x,y,z}, true });
        }
    }
    return v;
}

std::vector<Sample> makeTorus(GridMode m, float R=0.7f, float r=0.35f) {
    std::vector<Sample> v;
    bool solid = (m == GridMode::SOLID);
    float step = solid ? 0.025f : 0.018f;
    for (float u = 0; u < 2*PI; u += step) {
        float cu=cos(u), su=sin(u), udeg=u*(180/PI);
        for (float vv = 0; vv < 2*PI; vv += step) {
            if (!gridTest(m, udeg, vv*(180/PI))) continue;
            float cv=cos(vv), sv=sin(vv);
            Vec3 norm = Vec3{cv*cu, cv*su, sv}.normalized();
            v.push_back({ {(R+r*cv)*cu, (R+r*cv)*su, r*sv}, norm, true });
        }
    }
    return v;
}

std::vector<Sample> makeCone(GridMode m, float radius=1.0f, float height=2.0f) {
    std::vector<Sample> v;
    bool solid = (m == GridMode::SOLID);
    float us = solid ? 0.025f : 0.015f;
    float ts = solid ? 0.025f : 0.015f;
    for (float u = 0; u < 2*PI; u += us) {
        float cu=cos(u), su=sin(u), udeg=u*(180/PI);
        for (float t = 0; t <= 1.0f; t += ts) {
            if (!gridTest(m, udeg, t*180.0f)) continue;
            float rr = radius*(1-t);
            Vec3 norm = Vec3{cu*height, su*height, radius}.normalized();
            v.push_back({ {rr*cu, rr*su, height*t - height/2}, norm, true });
        }
    }
    return v;
}

std::vector<Sample> makeCylinder(GridMode m, float radius=0.8f, float height=1.8f) {
    std::vector<Sample> v;
    bool solid = (m == GridMode::SOLID);
    float us = solid ? 0.025f : 0.015f;
    float ts = solid ? 0.025f : 0.020f;
    for (float u = 0; u < 2*PI; u += us) {
        float cu=cos(u), su=sin(u), udeg=u*(180/PI);
        // superficie lateral
        for (float t = 0; t <= 1.0f; t += ts) {
            if (!gridTest(m, udeg, t*180.0f)) continue;
            v.push_back({ {radius*cu, radius*su, height*t - height/2}, {cu, su, 0}, true });
        }
        // tampas (topo e base)
        for (float r2 = 0; r2 <= radius; r2 += 0.07f) {
            bool g = (r2 < 0.07f || int(udeg) % 45 < 3);
            if (!g && !solid) continue;
            v.push_back({ {r2*cu, r2*su, -height/2}, {0,0,-1}, g });
            v.push_back({ {r2*cu, r2*su,  height/2}, {0,0, 1}, g });
        }
    }
    return v;
}

std::vector<Sample> buildSamples(Primitive p, GridMode m) {
    switch (p) {
        case Primitive::SPHERE:   return makeSphere(m);
        case Primitive::TORUS:    return makeTorus(m);
        case Primitive::CONE:     return makeCone(m);
        case Primitive::CYLINDER: return makeCylinder(m);
    }
    return makeSphere(m);
}

// ============================================================
// RENDERER
// ============================================================

struct Pixel { char ch; int r, g, b; };

class Renderer {
    int W, H;
    std::vector<Pixel> screen;
    std::vector<float> zbuf;
    const std::string shade = " .:-=+*#%@";

    void plot(int x, int y, float z, char ch, int r, int g, int b) {
        if (x < 0 || x >= W || y < 0 || y >= H) return;
        int i = y*W + x;
        if (z > zbuf[i]) {
            zbuf[i] = z;
            screen[i] = {ch, r, g, b};
        }
    }

public:
    Renderer(int w, int h) : W(w), H(h), screen(w*h), zbuf(w*h) {}

    void resize(int w, int h) {
        W = w; H = h;
        screen.assign(w*h, {' ', 5, 5, 15});
        zbuf.assign(w*h, 0.0f);
    }

    void clear() {
        std::fill(screen.begin(), screen.end(), Pixel{' ', 5, 5, 15});
        std::fill(zbuf.begin(), zbuf.end(), 0.0f);
    }

    void drawObjects(const std::vector<Sample>& samples, Vec3 rot,
                     const std::vector<Light>& lights, float dist, float k1) {
        const int slen = int(shade.size());
        for (const auto& s : samples) {
            Vec3 p = applyRot(s.pos, rot);
            Vec3 n = applyRot(s.normal, rot);

            float ooZ = 1.0f / (p.z + dist);
            if (ooZ <= 0) continue;

            int xp = int((W/2) + 2.2f * k1 * ooZ * p.x);
            int yp = int((H/2) -        k1 * ooZ * p.y);

            // Iluminacao com multiplas luzes coloridas
            float fr = 0.05f, fg = 0.05f, fb = 0.10f; // luz ambiente
            for (const auto& l : lights) {
                float d = std::max(0.0f, n.dot(l.dir.normalized()));
                fr += d * l.r * l.intensity;
                fg += d * l.g * l.intensity;
                fb += d * l.b * l.intensity;
            }
            fr = std::min(1.0f, fr);
            fg = std::min(1.0f, fg);
            fb = std::min(1.0f, fb);

            float lum = 0.2126f*fr + 0.7152f*fg + 0.0722f*fb;
            int ci = std::max(0, std::min(int(lum * slen), slen - 1));
            char ch = shade[ci];
            if (s.isGrid) ch = (ci > 5) ? '#' : '+';

            plot(xp, yp, ooZ, ch, int(fr*255), int(fg*255), int(fb*255));
        }
    }

    // Sombra projetada no chao usando a direcao da luz principal
    void drawShadow(const std::vector<Sample>& samples, Vec3 rot,
                    Vec3 lightDir, float dist, float k1) {
        Vec3 ld = lightDir.normalized();
        if (ld.y <= 1e-4f) return; // luz de baixo nao projeta sombra no chao

        const float floorY = -1.8f;
        for (const auto& s : samples) {
            Vec3 p = applyRot(s.pos, rot);

            float t = (p.y - floorY) / ld.y;
            if (t < 0) continue;

            Vec3 sp(p.x - t*ld.x, floorY, p.z - t*ld.z);
            float ooZ = 1.0f / (sp.z + dist);
            if (ooZ <= 0) continue;

            int xp = int((W/2) + 2.2f * k1 * ooZ * sp.x);
            int yp = int((H/2) -        k1 * ooZ * sp.y);

            // z reduzido para o objeto sempre ficar na frente da sombra
            plot(xp, yp, ooZ * 0.3f, '.', 12, 12, 30);
        }
    }

    void present(bool useColor, int fps, const std::string& status) {
        std::string out;
        out.reserve(W * H * (useColor ? 22 : 2));
        out += "\x1b[H\x1b[?25l";

        // Run-length encoding de cores: so emite novo codigo ANSI quando a cor muda
        int lr = -1, lg = -1, lb = -1;
        char buf[24];

        for (int y = 0; y < H - 1; ++y) {
            for (int x = 0; x < W; ++x) {
                const Pixel& p = screen[y*W + x];
                if (useColor && (p.r != lr || p.g != lg || p.b != lb)) {
                    snprintf(buf, sizeof(buf), "\x1b[38;2;%d;%d;%dm", p.r, p.g, p.b);
                    out += buf;
                    lr = p.r; lg = p.g; lb = p.b;
                }
                out += p.ch;
            }
            out += '\n';
        }
        if (useColor) out += "\x1b[0m";
        out += status;
        std::cout << out << std::flush;
    }
};

// ============================================================
// TAMANHO DO TERMINAL
// ============================================================

void termSize(int& w, int& h) {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        w = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        h = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
        return;
    }
#else
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0) {
        w = ws.ws_col; h = ws.ws_row; return;
    }
#endif
    w = 160; h = 50;
}

// ============================================================
// MAIN
// ============================================================

int main() {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE) {
        DWORD dwMode = 0;
        GetConsoleMode(hOut, &dwMode);
        SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }
#endif

    std::cout << "\x1b[2J\x1b[?25l";

    int sw, sh;
    termSize(sw, sh);
    Renderer renderer(sw, sh);

    Vec3  rot;
    float lightX = 0.5f, lightY = 1.0f;

    std::vector<Light> lights = {
        { {lightX, lightY, -0.7f}, 1.0f, 0.90f, 0.70f, 0.9f }, // luz principal quente (WASD)
        { {-0.8f,  0.3f,   0.5f}, 0.3f, 0.50f, 0.80f, 0.5f }, // luz de preenchimento fria (fixa)
    };

    bool   autoRot   = true;
    bool   useColor  = true;
    bool   useShadow = true;
    auto   prim      = Primitive::SPHERE;
    auto   gridMode  = GridMode::LATLON;
    bool   rebuild   = false;

    auto samples = buildSamples(prim, gridMode);

    int fps = 0, frames = 0;
    auto fpsTimer = std::chrono::steady_clock::now();

    while (true) {
        // --- ENTRADA ---
        rebuild = false;
        while (_kbhit()) {
            int key = _getch();

            if (key == 27 || key == 'q' || key == 'Q') {
                std::cout << "\x1b[2J\x1b[H\x1b[?25h"; // limpa e mostra cursor
                return 0;
            }

            if (key == ' ')                autoRot   = !autoRot;
            if (key == 'c' || key == 'C')  useColor  = !useColor;
            if (key == 'x' || key == 'X')  useShadow = !useShadow;

            // Controle da luz principal
            if (key == 'w' || key == 'W') lightY = std::min(lightY + 0.2f,  3.0f);
            if (key == 's' || key == 'S') lightY = std::max(lightY - 0.2f, -3.0f);
            if (key == 'a' || key == 'A') lightX = std::max(lightX - 0.2f, -3.0f);
            if (key == 'd' || key == 'D') lightX = std::min(lightX + 0.2f,  3.0f);
            lights[0].dir = {lightX, lightY, -0.7f};

            // Trocar primitiva
            if (key == '\t') {
                prim = static_cast<Primitive>((int(prim) + 1) % 4);
                rebuild = true;
            }
            // Trocar modo de grade
            if (key == 'g' || key == 'G') {
                gridMode = static_cast<GridMode>((int(gridMode) + 1) % 3);
                rebuild = true;
            }

            // Teclas de seta no Windows: primeiro byte 0 ou 224, segundo byte e a direcao
            if (key == 0 || key == 224) {
                int ext = _getch();
                if (ext == 72) rot.x -= 0.15f; // seta cima
                if (ext == 80) rot.x += 0.15f; // seta baixo
                if (ext == 77) rot.y += 0.15f; // seta direita
                if (ext == 75) rot.y -= 0.15f; // seta esquerda
            }
        }

        if (rebuild) {
            samples = buildSamples(prim, gridMode);
            std::cout << "\x1b[2J";
        }

        // --- REDIMENSIONAMENTO DO TERMINAL ---
        int nw, nh;
        termSize(nw, nh);
        if (nw != sw || nh != sh) {
            sw = nw; sh = nh;
            renderer.resize(sw, sh);
            std::cout << "\x1b[2J";
        }

        // --- ATUALIZACAO ---
        if (autoRot) {
            rot.x += 0.010f;
            rot.y += 0.030f;
            rot.z += 0.020f;
        }

        float k1   = sh * 2.2f;
        float dist = 5.0f;

        // --- RENDER ---
        renderer.clear();
        if (useShadow) renderer.drawShadow(samples, rot, lights[0].dir, dist, k1);
        renderer.drawObjects(samples, rot, lights, dist, k1);

        // --- FPS ---
        ++frames;
        auto now = std::chrono::steady_clock::now();
        auto ms  = std::chrono::duration_cast<std::chrono::milliseconds>(now - fpsTimer).count();
        if (ms >= 1000) {
            fps    = int(frames * 1000 / ms);
            frames = 0;
            fpsTimer = now;
        }

        char sfps[16];
        snprintf(sfps, sizeof(sfps), " FPS:%d", fps);

        std::string status = sfps;
        status += " | [WASD] Luz | [Setas] Rotacao | [Espaco] Auto:";
        status += autoRot ? "ON " : "OFF";
        status += " | [TAB] ";
        status += primNames[int(prim)];
        status += " | [G] ";
        status += gridNames[int(gridMode)];
        status += " | [C] Cor | [X] Sombra | [Q] Sair    ";

        renderer.present(useColor, fps, status);

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}
