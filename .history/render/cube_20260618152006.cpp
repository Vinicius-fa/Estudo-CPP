#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <thread>
#include <chrono>
#include <cstring>

using namespace std;

constexpr double PI = 3.14159265358979323846;

// Estrutura para representar um ponto 3D
struct Point3D {
    double x, y, z;
    
    Point3D(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}
};

// Estrutura para representar um ponto 2D (projeção na tela)
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
    const int SCREEN_WIDTH = 80;
    const int SCREEN_HEIGHT = 24;
    const double SCALE = 7.5;
    const double FOV_DISTANCE = 50.0;
    
public:
    RotatingCube() {
        // Inicializa os 8 vértices do cubo
        initializeVertices();
    }
    
    void initializeVertices() {
        vertices.clear();
        double size = 1.0;
        
        // Os 8 vértices do cubo
        vertices.push_back(Point3D(-size, -size, -size));
        vertices.push_back(Point3D(size, -size, -size));
        vertices.push_back(Point3D(size, size, -size));
        vertices.push_back(Point3D(-size, size, -size));
        vertices.push_back(Point3D(-size, -size, size));
        vertices.push_back(Point3D(size, -size, size));
        vertices.push_back(Point3D(size, size, size));
        vertices.push_back(Point3D(-size, size, size));
    }
    
    // Matrizes de rotação
    Point3D rotateX(const Point3D& p, double angle) {
        double rad = angle * PI / 180.0;
        double cos_a = cos(rad);
        double sin_a = sin(rad);
        
        return Point3D(
            p.x,
            p.y * cos_a - p.z * sin_a,
            p.y * sin_a + p.z * cos_a
        );
    }
    
    Point3D rotateY(const Point3D& p, double angle) {
        double rad = angle * PI / 180.0;
        double cos_a = cos(rad);
        double sin_a = sin(rad);
        
        return Point3D(
            p.x * cos_a + p.z * sin_a,
            p.y,
            -p.x * sin_a + p.z * cos_a
        );
    }
    
    Point3D rotateZ(const Point3D& p, double angle) {
        double rad = angle * PI / 180.0;
        double cos_a = cos(rad);
        double sin_a = sin(rad);
        
        return Point3D(
            p.x * cos_a - p.y * sin_a,
            p.x * sin_a + p.y * cos_a,
            p.z
        );
    }
    
    // Projeção perspectiva 3D -> 2D
    Point2D project(const Point3D& p) {
        // Aplica perspectiva
        double scale = FOV_DISTANCE / (FOV_DISTANCE + p.z);
        
        // Multiplicamos o eixo X por 2.0 para compensar que os caracteres do terminal
        // são tipicamente mais altos do que largos (Correção de Aspect Ratio)
        double x2d = p.x * scale * SCALE * 2.0 + SCREEN_WIDTH / 2.0;
        double y2d = p.y * scale * SCALE + SCREEN_HEIGHT / 2.0;
        
        return Point2D(x2d, y2d, p.z, '#');
    }
    
    void update() {
        // Incrementa os ângulos de rotação
        angleX += 2.0;
        angleY += 3.0;
        angleZ += 1.5;
        
        // Se exceder 360, reseta
        if (angleX >= 360) angleX -= 360;
        if (angleY >= 360) angleY -= 360;
        if (angleZ >= 360) angleZ -= 360;
        
        // Aplica rotações a cada vértice
        projectedVertices.clear();
        for (const auto& vertex : vertices) {
            Point3D rotated = vertex;
            rotated = rotateX(rotated, angleX);
            rotated = rotateY(rotated, angleY);
            rotated = rotateZ(rotated, angleZ);
            
            projectedVertices.push_back(project(rotated));
        }

        // CORREÇÃO: O bloco std::sort foi removido daqui para manter os índices
        // dos vértices fixos em relação ao mapeamento do array de arestas.
    }
    
    void draw() {
    // Cria buffer da tela
    vector<vector<char>> screen(SCREEN_HEIGHT, vector<char>(SCREEN_WIDTH, ' '));
    
    // Desenha as arestas primeiro
    drawEdges(screen);

    // Desenha os vértices por cima
    for (const auto& p : projectedVertices) {
        int x = static_cast<int>(p.x);
        int y = static_cast<int>(p.y);
        
        if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
            screen[y][x] = p.ch;
        }
    }
    
    // CORREÇÃO AQUI: 
    // \033[H move o cursor para o topo (0,0) sem apagar nada.
    // \033[?25l esconde o cursor piscante do terminal para não atrapalhar.
    cout << "\033[H\033[?25l"; 
    
    // Cria uma string gigante para mandar tudo de uma vez só para o terminal.
    // Isso reduz drasticamente o lag de comunicação do console.
    string output = "";
    for (int y = 0; y < SCREEN_HEIGHT; ++y) {
        for (int x = 0; x < SCREEN_WIDTH; ++x) {
            output += screen[y][x];
        }
        output += "\n";
    }
    
    cout << output;
    cout << "Use Ctrl+C para sair. FPS estimado: 20   " << endl;
}
    
    void drawEdges(vector<vector<char>>& screen) {
        // Define as arestas fixas do cubo (mapeando os índices originais)
        int edges[12][2] = {
            // Face frontal
            {0, 1}, {1, 2}, {2, 3}, {3, 0},
            // Face traseira
            {4, 5}, {5, 6}, {6, 7}, {7, 4},
            // Arestas conectando frente com trás
            {0, 4}, {1, 5}, {2, 6}, {3, 7}
        };
        
        for (int i = 0; i < 12; ++i) {
            drawLine(screen, 
                     projectedVertices[edges[i][0]],
                     projectedVertices[edges[i][1]]);
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
            if (x0 >= 0 && x0 < SCREEN_WIDTH && y0 >= 0 && y0 < SCREEN_HEIGHT) {
                screen[y0][x0] = '-';
            }
            
            if (x0 == x1 && y0 == y1) break;
            
            int e2 = 2 * err;
            if (e2 > -dy) {
                err -= dy;
                x0 += sx;
            }
            if (e2 < dx) {
                err += dx;
                y0 += sy;
            }
        }
    }
};

int main() {
    // Executa apenas se o terminal suportar sequências de escape ANSI para limpar a tela
    RotatingCube cube;
    
    while (true) {
        cube.update();
        cube.draw();
        std::this_thread::sleep_for(std::chrono::milliseconds(50)); // ~20 FPS
    }
    
    return 0;
}