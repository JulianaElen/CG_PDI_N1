// Elo Maluco
// cube.cpp

#include "cube.hpp"

Cube::Cube(const vector<vector<string>>& colorsMatrix): colorsMatrix(colorsMatrix)
{
    x = y = z = 0;
    theta = 0.0f;

    // vertices da fase frontal
    points_base[0] = vec3(15.0f, 15.0f, 15.0f);
    points_base[1] = vec3(15.0f, 0.0f, 15.0f);
    points_base[2] = vec3(0.0f, 0.0f, 15.0f);
    points_base[3] = vec3(0.0f, 15.0f, 15.0f);

    // vertices da fase de traz
    points_base[4] = vec3(15.0f, 15.0f, 0.0f);
    points_base[5] = vec3(15.0f, 0.0f, 0.0f);
    points_base[6] = vec3(0.0f, 0.0f, 0.0f);
    points_base[7] = vec3(0.0f, 15.0f, 0.0f);

    for (int i = 0; i < 8; i++)
    {
        points[i] = points_base[i];
    }
}

Cube::~Cube() {}

void Cube::transform(void)
{
    mat3 Model = mat3(1.0);
    Model[2] = vec3(x, y, z);

    for (int i = 0; i < 8; i++)
    {
        points[i] = Model * points_base[i];
    }
}

void Cube::update(int value)
{
    x += value;
    y += value;
    z += value;

    transform();
}

//---------------------------------------------------------------------
// Função de desenho dos cubos com bordas
void Cube::draw()
{
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(theta, 0.0f, 1.0f, 0.0f);

// Itera sobre a matriz de strings (colorsMatrix) em ordem inversa
    for (size_t i = colorsMatrix.size(); i > 0; --i) // Começa do final e vai até 1
    {
        vector<Color> rgbColors;
        for (size_t j = 0; j < colorsMatrix[i - 1].size(); ++j) // i-1 para acessar corretamente
        {
            Color rgbColor = mapColor(colorsMatrix[i - 1][j]);
            rgbColors.push_back(rgbColor); // Armazena a cor no vetor
        }
        // Desenha o cubo usando as cores armazenadas
        drawCube(rgbColors); // Supondo que drawCube aceite um vetor de cores

        // Movimenta a posição para o próximo cubo (ou face)
        glTranslatef(0.0f, 15.0f, 0.0f); // Exemplo de deslocamento em Y
    }
    glPopMatrix();
}

//---------------------------------------------------------------------
// Desenho de um cubo com as seis faces e bordas pretas
void Cube::drawCube(const vector<Color>& colors)
{
        
        // Desenha as seis faces do cubo
    glBegin(GL_QUADS);

        // Face frontal
        glColor3f(colors[0].r, colors[0].g, colors[0].b);
        glVertex3f(points_base[0].x, points_base[0].y, points_base[0].z);
        glVertex3f(points_base[1].x, points_base[1].y, points_base[1].z);
        glVertex3f(points_base[2].x, points_base[2].y, points_base[2].z);
        glVertex3f(points_base[3].x, points_base[3].y, points_base[3].z);

        // Face lateral 1
        glColor3f(colors[1].r, colors[1].g, colors[1].b);
        glVertex3f(points_base[0].x, points_base[0].y, points_base[0].z);
        glVertex3f(points_base[1].x, points_base[1].y, points_base[1].z);
        glVertex3f(points_base[5].x, points_base[5].y, points_base[5].z);
        glVertex3f(points_base[4].x, points_base[4].y, points_base[4].z);

        // Face traseira
        glColor3f(colors[2].r, colors[2].g, colors[2].b);
        glVertex3f(points_base[4].x, points_base[4].y, points_base[4].z);
        glVertex3f(points_base[5].x, points_base[5].y, points_base[5].z);
        glVertex3f(points_base[6].x, points_base[6].y, points_base[6].z);
        glVertex3f(points_base[7].x, points_base[7].y, points_base[7].z);

        // Face lateral 2
        glColor3f(colors[3].r, colors[3].g, colors[3].b);
        glVertex3f(points_base[3].x, points_base[3].y, points_base[3].z);
        glVertex3f(points_base[2].x, points_base[2].y, points_base[2].z);
        glVertex3f(points_base[6].x, points_base[6].y, points_base[6].z);
        glVertex3f(points_base[7].x, points_base[7].y, points_base[7].z);

        // Face superior 
        glColor3f(0.3f, 0.3f, 0.3f); 
        glVertex3f(points_base[0].x, points_base[0].y, points_base[0].z);
        glVertex3f(points_base[3].x, points_base[3].y, points_base[3].z);
        glVertex3f(points_base[7].x, points_base[7].y, points_base[7].z);
        glVertex3f(points_base[4].x, points_base[4].y, points_base[4].z);

        // Face inferior 
        glColor3f(0.3f, 0.3f, 0.3f); 
        glVertex3f(points_base[1].x, points_base[1].y, points_base[1].z);
        glVertex3f(points_base[2].x, points_base[2].y, points_base[2].z);
        glVertex3f(points_base[6].x, points_base[6].y, points_base[6].z);
        glVertex3f(points_base[5].x, points_base[5].y, points_base[5].z);
    glEnd();
    
    // Desenha as 12 bordas pretas no cubo, para boa aparencia
        glColor3f(0.0f, 0.0f, 0.0f); 
        glBegin(GL_LINES);


        glVertex3f(points_base[0].x, points_base[0].y, points_base[0].z);
        glVertex3f(points_base[1].x, points_base[1].y, points_base[1].z);

        glVertex3f(points_base[1].x, points_base[1].y, points_base[1].z);
        glVertex3f(points_base[2].x, points_base[2].y, points_base[2].z);

        glVertex3f(points_base[2].x, points_base[2].y, points_base[2].z);
        glVertex3f(points_base[3].x, points_base[3].y, points_base[3].z);

        glVertex3f(points_base[3].x, points_base[3].y, points_base[3].z);
        glVertex3f(points_base[0].x, points_base[0].y, points_base[0].z);

        glVertex3f(points_base[4].x, points_base[4].y, points_base[4].z);
        glVertex3f(points_base[5].x, points_base[5].y, points_base[5].z);

        glVertex3f(points_base[5].x, points_base[5].y, points_base[5].z);
        glVertex3f(points_base[6].x, points_base[6].y, points_base[6].z);

        glVertex3f(points_base[6].x, points_base[6].y, points_base[6].z);
        glVertex3f(points_base[7].x, points_base[7].y, points_base[7].z);

        glVertex3f(points_base[7].x, points_base[7].y, points_base[7].z);
        glVertex3f(points_base[4].x, points_base[4].y, points_base[4].z);


        glVertex3f(points_base[0].x, points_base[0].y, points_base[0].z);
        glVertex3f(points_base[4].x, points_base[4].y, points_base[4].z);

        glVertex3f(points_base[1].x, points_base[1].y, points_base[1].z);
        glVertex3f(points_base[5].x, points_base[5].y, points_base[5].z);

        glVertex3f(points_base[2].x, points_base[2].y, points_base[2].z);
        glVertex3f(points_base[6].x, points_base[6].y, points_base[6].z);

        glVertex3f(points_base[3].x, points_base[3].y, points_base[3].z);
        glVertex3f(points_base[7].x, points_base[7].y, points_base[7].z);

        glEnd();  

}

//---------------------------------------------------------------------
// Mapeamento de string para RGB
Color Cube::mapColor(const string &code)
{
    if (code == "vms" || code == "vmi" || code == "vmm")
        return {1.0f, 0.0f, 0.0f}; // vermelho
    if (code == "vds" || code == "vdi" || code == "vdm" || code == "vrs" || code == "vri" || code == "vrm")
        return {0.0f, 1.0f, 0.0f}; // verde
    if (code == "ams" || code == "ami" || code == "amm")
        return {1.0f, 1.0f, 0.0f}; // amarelo
    if (code == "brs" || code == "bri" || code == "brm")
        return {1.0f, 1.0f, 1.0f}; // branco
    if (code == "vzo")
        return {0.5f, 0.5f, 0.5f}; // cinza
    return {0.0f, 0.0f, 0.0f};     // preto (default)
}
//---------------------------------------------------------------------
// Define a nova matriz de cores
void Cube::setColors(const vector<vector<string>>& newColorMatrix)
{
    colorsMatrix = newColorMatrix;
}

void Cube::colision(int x, int y) {}
