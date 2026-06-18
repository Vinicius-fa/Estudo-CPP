#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <thread>
#include <chrono>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

using namespace std;

constexpr double PI = 3.14159265358979323846;

struct Point3D {
    double x, y, z;
    Point3D(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}
};

struct Point2D {
    double x, y, depth;
    char ch;
    Point2D(double x = 0, double y = 0, double depth = 0, char ch = ' ') 
        : x(x), y(y), depth(depth), ch(ch) {}
};

class RotatingCube {
private:
    vector<Point3D> vertices;
    vector<Point2D> projectedVertices;
    double angleX = 0, angleY = 0, angleZ = 0;
    
    // Deixamos de ser const para atualizar dinamicamente
    int screen_width = 80;
    int screen_height = 24;
    
    const double SCALE = 10.0; // Diminuído levemente para ficar melhor em resoluções maiores
    const double FOV_DISTANCE = 60.0;
    
public:
    RotatingCube() {
        initializeVertices();
    }
    
    void initializeVertices() {
        vertices.clear();
        double size = 1.0;
        vertices.push_back(Point3D(-size, -size, -size));
        vertices.push_back(Point3D(size, -size, -size));
        vertices.push_back(Point3D(size, size, -size));
        vertices.push_back(Point3D(-size, size, -size));
        vertices.push_back(Point3D(-size, -size, size));
        vertices.push_back(Point3D(size, -size, size));
        vertices.push_back(Point3D(size, size, size));
        vertices.push_back(Point3D(-size, size, size));
    }
    
    // Função para pegar o tamanho real da janela do terminal atual
    void updateTerminalSize() {
#ifdef _WIN32
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
            screen_width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
            screen_height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
        }
#else
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        screen_width = w.ws_col;
        screen_height = w.ws_row;
#endif
        // Garante valores mínimos para não quebrar o vetor
        if (screen_width < 10) screen_width = 80;
        if (screen_height < 10) screen_height = 24;
    }
    
    Point3D rotateX(const Point3D& p, double angle) {
        double rad = angle * PI / 180.0;
        double cos_a = cos(rad); double sin_a = sin(rad);
        return Point3D(p.x, p.y * cos_a - p.z * sin_a, p.y * sin_a + p.z * cos_a);
    }
    
    Point3D rotateY(const Point3D& p, double angle) {
        double rad = angle * PI / 180.0;
        double cos_a = cos(rad); double sin_a = sin(rad);
        return Point3D(p.x * cos_a + p.z * sin_a, p.y, -p.x * sin_a + p.z * cos_a);
    }
    
    Point3D rotateZ(const Point3D& p, double angle) {
        double rad = angle * PI / 180.0;
        double cos_a = cos(rad); double sin_a = sin(rad);
        return Point3D(p.x * cos_a - p.y * sin_a, p.x * sin_a + p.y * cos_a, p.z);
    }
    
    Point2D project(const Point3D& p) {
        double scale = FOV_DISTANCE / (FOV_DISTANCE + p.z);
        
        // Aqui usamos screen_width e screen_height atualizados dinamicamente!
        double x2d = p.x * scale * SCALE * 2.2 + screen_width / 2.0;
        double y2d = p.y * scale * SCALE + screen_height / 2.0;
        
        return Point2D(x2d, y2d, p.z, '#');
    }
    
    void update() {
        updateTerminalSize(); // Verifica o tamanho do terminal antes de calcular o próximo frame
        
        angleX += 2.0;
        angleY += 3.0;
        angleZ += 1.5;
        
        if (angleX >= 360) angleX -= 360;
        if (angleY >= 360) angleY -= 360;
        if (angleZ >= 360) angleZ -= 360;
        
        projectedVertices.clear();
        for (const auto& vertex : vertices) {
            Point3D rotated = vertex;
            rotated = rotateX(rotated, angleX);
            rotated = rotateY(rotated, angleY);
            rotated = rotateZ(rotated, angleZ);
            projectedVertices.push_back(project(rotated));
        }
    }
    
    void draw() {
        // Aloca o buffer exatamente no tamanho atual da tela
        vector<vector<char>> screen(screen_height, vector<char>(screen_width, ' '));
        
        drawEdges(screen);

        for (const auto& p : projectedVertices) {
            int x = static_cast<int>(p.x);
            int y = static_cast<int>(p.y);
            if (x >= 0 && x < screen_width && y >= 0 && y < screen_height) {
                screen[y][x] = p.ch;
            }
        }
        
        // Volta para o topo da tela e esconde o cursor
        cout << "\033[H\033[?25l"; 
        
        string output = "";
        // Desenhamos apenas até screen_height - 1 para evitar que o terminal role para baixo
        for (int y = 0; y < screen_height - 1; ++y) {
            for (int x = 0; x < screen_width; ++x) {
                output += screen[y][x];
            }
            output += "\n";
        }
        
        cout << output << flush;
    }
    
    void drawEdges(vector<vector<char>>& screen) {
        int edges[12][2] = {
            {0, 1}, {1, 2}, {2, 3}, {3, 0},
            {4, 5}, {5, 6}, {6, 7}, {7, 4},
            {0, 4}, {1, 5}, {2, 6}, {3, 7}
        };
        
        for (int i = 0; i < 12; ++i) {
            drawLine(screen, projectedVertices[edges[i][0]], projectedVertices[edges[i][1]]);
        }
    }
    
    void drawLine(vector<vector<char>>& screen, const Point2D& p1, const Point2D& p2) {
        int x0 = static_cast<int>(p1.x);
        int y0 = static_cast<int>(p1.y);
        int x1 = static_cast<int>(p2.x);
        int y1 = static_cast<int>(p2.y);
        
        int dx = abs(x1 - x0);
        int dy = abs(y1 - y0);
        int sx = (x0 < x1) ? 1 : -1;
        int sy = (y0 < y1) ? 1 : -1;
        int err = dx - dy;
        
        while (true) {
            if (x0 >= 0 && x0 < screen_width && y0 >= 0 && y0 < screen_height) {
                screen[y0][x0] = '-';
            }
            if (x0 == x1 && y0 == y1) break;
            int e2 = 2 * err;
            if (e2 > -dy) { err -= dy; x0 += sx; }
            if (e2 < dx) { err += dx; y0 += sy; }
        }
    }
};

int main() {
    RotatingCube cube;
    
    // Limpa a tela inteira uma única vez antes de começar
    cout << "\033[2J"; 
    
    while (true) {
        cube.update();
        cube.draw();
        std::this_thread::sleep_for(std::chrono::milliseconds(40));
    }
    return 0;
}