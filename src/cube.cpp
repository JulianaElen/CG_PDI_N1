// Elo Maluco
// cube.cpp

#include "cube.hpp"

Cube::Cube()
{
    x = y = z = 0;
    theta = 0.0f;

    //vertices da fase frontal
    points_base[0] = glm::vec3(15.0f, 15.0f, 15.0f); 
    points_base[1] = glm::vec3(15.0f, 0.0f, 15.0f);
    points_base[2] = glm::vec3(0.0f, 0.0f, 15.0f);
    points_base[3] = glm::vec3(0.0f, 15.0f, 15.0f);

    //vertices da fase de traz
    points_base[4] = glm::vec3(15.0f, 15.0f, 0.0f);
    points_base[5] = glm::vec3(15.0f, 0.0f, 0.0f);
    points_base[6] = glm::vec3(0.0f, 0.0f, 0.0f);
    points_base[7] = glm::vec3(0.0f, 15.0f, 0.0f);

    for (int i = 0; i < 8; i++) {
        points[i] = points_base[i];
    }
}

Cube::~Cube() {}

void Cube::transform(void)
{
    glm::mat3 Model = glm::mat3(1.0);
    Model[2] = glm::vec3(x, y, z);

    for (int i = 0; i < 8; i++) {
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

void Cube::draw()
{glLineWidth(3.0f);

    glBegin(GL_QUADS);
    
    // Face frontal
    glColor3f(1.0f, 0.0f, 0.0f); // Vermelho
    glVertex3f(points[0].x, points[0].y, points[0].z);
    glVertex3f(points[1].x, points[1].y, points[1].z);
    glVertex3f(points[2].x, points[2].y, points[2].z);
    glVertex3f(points[3].x, points[3].y, points[3].z);

    // Face traseira
    glColor3f(0.0f, 1.0f, 0.0f); // Verde
    glVertex3f(points[4].x, points[4].y, points[4].z);
    glVertex3f(points[5].x, points[5].y, points[5].z);
    glVertex3f(points[6].x, points[6].y, points[6].z);
    glVertex3f(points[7].x, points[7].y, points[7].z);

    // Face superior
    glColor3f(1.0f, 0.0f, 1.0f); // Magenta
    glVertex3f(points[0].x, points[0].y, points[0].z);
    glVertex3f(points[3].x, points[3].y, points[3].z);
    glVertex3f(points[7].x, points[7].y, points[7].z);
    glVertex3f(points[4].x, points[4].y, points[4].z);
    
    // Face inferior
    glColor3f(0.0f, 1.0f, 1.0f); // Ciano
    glVertex3f(points[1].x, points[1].y, points[1].z);
    glVertex3f(points[2].x, points[2].y, points[2].z);
    glVertex3f(points[6].x, points[6].y, points[6].z);
    glVertex3f(points[5].x, points[5].y, points[5].z);

        // Face 
    glColor3f(1.0f, 1.0f, 0.0f); // Amarelo
    glVertex3f(points[0].x, points[0].y, points[0].z);
    glVertex3f(points[1].x, points[1].y, points[1].z);
    glVertex3f(points[5].x, points[5].y, points[5].z);
    glVertex3f(points[4].x, points[4].y, points[4].z);

    // Face 
    glColor3f(0.0f, 0.0f, 1.0f); // Azul
    glVertex3f(points[3].x, points[3].y, points[3].z);
    glVertex3f(points[2].x, points[2].y, points[2].z);
    glVertex3f(points[6].x, points[6].y, points[6].z);
    glVertex3f(points[7].x, points[7].y, points[7].z);

    glEnd();
}

void Cube::colision(int x, int y) {}
