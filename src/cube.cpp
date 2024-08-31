// Elo Maluco
// cube.cpp

#include "cube.hpp"

Cube::Cube()
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

    for (int i = 0; i < 4; i++) // Desenhar 4 cubos empilhados
    {
        glPushMatrix(); // Salva a matriz de transformação atual

        glTranslatef(0.0f, i * 15.0f, 0.0f); // desloca o cubo ao longo do eixo Y

        glBegin(GL_QUADS);

        // Face frontal
        glColor3f(1.0f, 0.0f, 0.0f); // Vermelho
        glVertex3f(points_base[0].x, points_base[0].y, points_base[0].z);
        glVertex3f(points_base[1].x, points_base[1].y, points_base[1].z);
        glVertex3f(points_base[2].x, points_base[2].y, points_base[2].z);
        glVertex3f(points_base[3].x, points_base[3].y, points_base[3].z);

        // Face traseira
        glColor3f(0.0f, 1.0f, 0.0f); // Verde
        glVertex3f(points_base[4].x, points_base[4].y, points_base[4].z);
        glVertex3f(points_base[5].x, points_base[5].y, points_base[5].z);
        glVertex3f(points_base[6].x, points_base[6].y, points_base[6].z);
        glVertex3f(points_base[7].x, points_base[7].y, points_base[7].z);

        // Face superior
        glColor3f(1.0f, 0.0f, 1.0f); // Magenta
        glVertex3f(points_base[0].x, points_base[0].y, points_base[0].z);
        glVertex3f(points_base[3].x, points_base[3].y, points_base[3].z);
        glVertex3f(points_base[7].x, points_base[7].y, points_base[7].z);
        glVertex3f(points_base[4].x, points_base[4].y, points_base[4].z);

        // Face inferior
        glColor3f(0.0f, 1.0f, 1.0f); // Ciano
        glVertex3f(points_base[1].x, points_base[1].y, points_base[1].z);
        glVertex3f(points_base[2].x, points_base[2].y, points_base[2].z);
        glVertex3f(points_base[6].x, points_base[6].y, points_base[6].z);
        glVertex3f(points_base[5].x, points_base[5].y, points_base[5].z);

        // Face lateral 1
        glColor3f(1.0f, 1.0f, 0.0f); // Amarelo
        glVertex3f(points_base[0].x, points_base[0].y, points_base[0].z);
        glVertex3f(points_base[1].x, points_base[1].y, points_base[1].z);
        glVertex3f(points_base[5].x, points_base[5].y, points_base[5].z);
        glVertex3f(points_base[4].x, points_base[4].y, points_base[4].z);

        // Face lateral 2
        glColor3f(0.0f, 0.0f, 1.0f); // Azul
        glVertex3f(points_base[3].x, points_base[3].y, points_base[3].z);
        glVertex3f(points_base[2].x, points_base[2].y, points_base[2].z);
        glVertex3f(points_base[6].x, points_base[6].y, points_base[6].z);
        glVertex3f(points_base[7].x, points_base[7].y, points_base[7].z);

        glEnd();
        glPopMatrix(); // restaura a matriz de transformação, para poder gerar o proximo cubo da
    }
}

void Cube::colision(int x, int y) {}
