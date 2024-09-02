// Elo Maluco
// cube.cpp

#include "cube.hpp"

Cube::Cube(const std::vector<std::vector<glm::vec3>>& colorsMatrix): colorsMatrix(colorsMatrix)
{
    x = y = z = 0;
    theta = 0.0f;

    // vertices da fase frontal
    points_base[0] = glm::vec3(15.0f, 15.0f, 15.0f);
    points_base[1] = glm::vec3(15.0f, 0.0f, 15.0f);
    points_base[2] = glm::vec3(0.0f, 0.0f, 15.0f);
    points_base[3] = glm::vec3(0.0f, 15.0f, 15.0f);

    // vertices da fase de traz
    points_base[4] = glm::vec3(15.0f, 15.0f, 0.0f);
    points_base[5] = glm::vec3(15.0f, 0.0f, 0.0f);
    points_base[6] = glm::vec3(0.0f, 0.0f, 0.0f);
    points_base[7] = glm::vec3(0.0f, 15.0f, 0.0f);

    for (int i = 0; i < 8; i++)
    {
        points[i] = points_base[i];
    }
}

Cube::~Cube() {}

void Cube::transform(void)
{
    glm::mat3 Model = glm::mat3(1.0);
    Model[2] = glm::vec3(x, y, z);

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

// gerar 4 cubos um em cima do outro:
void Cube::draw()
{
    glLineWidth(3.0f);

    // Iterar sobre a matriz de cores para desenhar os cubos
    for (int i = 0; i < colorsMatrix.size(); ++i)
    {
        glPushMatrix(); 

        glTranslatef(0.0f, i * 15.0f, 0.0f); // desloca o cubo ao longo do eixo Y

        glBegin(GL_QUADS);

        // Face frontal
        glColor3f(colorsMatrix[i][0].r, colorsMatrix[i][0].g, colorsMatrix[i][0].b); 
        glVertex3f(points_base[0].x, points_base[0].y, points_base[0].z);
        glVertex3f(points_base[1].x, points_base[1].y, points_base[1].z);
        glVertex3f(points_base[2].x, points_base[2].y, points_base[2].z);
        glVertex3f(points_base[3].x, points_base[3].y, points_base[3].z);

        // Face traseira
        glColor3f(colorsMatrix[i][1].r, colorsMatrix[i][1].g, colorsMatrix[i][1].b); 
        glVertex3f(points_base[4].x, points_base[4].y, points_base[4].z);
        glVertex3f(points_base[5].x, points_base[5].y, points_base[5].z);
        glVertex3f(points_base[6].x, points_base[6].y, points_base[6].z);
        glVertex3f(points_base[7].x, points_base[7].y, points_base[7].z);

        // Face superior - sempre preta
        glColor3f(0.3f, 0.3f, 0.3f); // Preto
        glVertex3f(points_base[0].x, points_base[0].y, points_base[0].z);
        glVertex3f(points_base[3].x, points_base[3].y, points_base[3].z);
        glVertex3f(points_base[7].x, points_base[7].y, points_base[7].z);
        glVertex3f(points_base[4].x, points_base[4].y, points_base[4].z);

        // Face inferior - sempre preta
        glColor3f(0.3f, 0.3f, 0.3f); // Preto
        glVertex3f(points_base[1].x, points_base[1].y, points_base[1].z);
        glVertex3f(points_base[2].x, points_base[2].y, points_base[2].z);
        glVertex3f(points_base[6].x, points_base[6].y, points_base[6].z);
        glVertex3f(points_base[5].x, points_base[5].y, points_base[5].z);

        // Face lateral 1
        glColor3f(colorsMatrix[i][2].r, colorsMatrix[i][2].g, colorsMatrix[i][2].b); 
        glVertex3f(points_base[0].x, points_base[0].y, points_base[0].z);
        glVertex3f(points_base[1].x, points_base[1].y, points_base[1].z);
        glVertex3f(points_base[5].x, points_base[5].y, points_base[5].z);
        glVertex3f(points_base[4].x, points_base[4].y, points_base[4].z);

        // Face lateral 2
        glColor3f(colorsMatrix[i][3].r, colorsMatrix[i][3].g, colorsMatrix[i][3].b); 
        glVertex3f(points_base[3].x, points_base[3].y, points_base[3].z);
        glVertex3f(points_base[2].x, points_base[2].y, points_base[2].z);
        glVertex3f(points_base[6].x, points_base[6].y, points_base[6].z);
        glVertex3f(points_base[7].x, points_base[7].y, points_base[7].z);

        glEnd();  

         // Desenha as bordas do cubo
        glColor3f(0.0f, 0.0f, 0.0f); // Cor das bordas
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

        glPopMatrix(); // restaura a matriz de transformação
    }
}

void Cube::colision(int x, int y) {}
