// Elo Maluco
// cube.cpp

#include "cube.hpp"

Cube::Cube(const vector<vector<vec3>>& colorsMatrix): colorsMatrix(colorsMatrix)
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

// gerar 4 cubos um em cima do outro:
void Cube::draw()
{
    glLineWidth(3.0f);

    // Iterar sobre a matriz de cores para desenhar os cubos, pois a matriz vai ter sempre as 4 linhas
    for (int i = 0; i < colorsMatrix.size(); ++i)
    {
    
        float positionY = (colorsMatrix.size() - 1 - i) * 15.0f; // Inverte a ordem das cores ao desenhar o cubo

        glPushMatrix(); 

        glTranslatef(0.0f, positionY, 0.0f); // desloca o cubo ao longo do eixo Y em 15 unidades

        // O restante do código permanece o mesmo
        glBegin(GL_QUADS);

        // Face frontal
        glColor3f(colorsMatrix[i][0].r, colorsMatrix[i][0].g, colorsMatrix[i][0].b); 
        glVertex3f(points_base[0].x, points_base[0].y, points_base[0].z);
        glVertex3f(points_base[1].x, points_base[1].y, points_base[1].z);
        glVertex3f(points_base[2].x, points_base[2].y, points_base[2].z);
        glVertex3f(points_base[3].x, points_base[3].y, points_base[3].z);

        // Face lateral 1
        glColor3f(colorsMatrix[i][1].r, colorsMatrix[i][1].g, colorsMatrix[i][1].b); 
        glVertex3f(points_base[0].x, points_base[0].y, points_base[0].z);
        glVertex3f(points_base[1].x, points_base[1].y, points_base[1].z);
        glVertex3f(points_base[5].x, points_base[5].y, points_base[5].z);
        glVertex3f(points_base[4].x, points_base[4].y, points_base[4].z);

        // Face traseira
        glColor3f(colorsMatrix[i][2].r, colorsMatrix[i][2].g, colorsMatrix[i][2].b); 
        glVertex3f(points_base[4].x, points_base[4].y, points_base[4].z);
        glVertex3f(points_base[5].x, points_base[5].y, points_base[5].z);
        glVertex3f(points_base[6].x, points_base[6].y, points_base[6].z);
        glVertex3f(points_base[7].x, points_base[7].y, points_base[7].z);

        // Face lateral 2
        glColor3f(colorsMatrix[i][3].r, colorsMatrix[i][3].g, colorsMatrix[i][3].b); 
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

        glPopMatrix(); // restaura a matriz de transformação
    }
}

// função para atualizar a matrix de cores passada
void Cube::setColors(const vector<vector<vec3>>& newColorsMatrix) {
    colorsMatrix = newColorsMatrix;
}

void Cube::colision(int x, int y) {}
