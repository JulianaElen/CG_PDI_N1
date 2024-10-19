// Elo Maluco
// cube.cpp

#include "cube.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Cube::Cube(const vector<vector<string>> &colorsMatrix) : colorsMatrix(colorsMatrix)
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
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(theta, 0.0f, 1.0f, 0.0f);

    for (size_t i = colorsMatrix.size(); i > 0; --i)
    {
        vector<Color> rgbColors;
        vector<GLuint> textures;

        for (size_t j = 0; j < colorsMatrix[i - 1].size(); ++j)
        {
            Color rgbColor = mapColor(colorsMatrix[i - 1][j]);
            rgbColors.push_back(rgbColor);

            GLuint textureID = mapTexture(colorsMatrix[i - 1][j]);
            textures.push_back(textureID);
        }

        drawCube(rgbColors, textures);

        glTranslatef(0.0f, 15.0f, 0.0f); // Move para próximo cubo
    }
    glPopMatrix();
}

//---------------------------------------------------------------------
// Desenho de um cubo com as seis faces e bordas pretas
void Cube::drawCube(const vector<Color> &colors, const vector<GLuint> &textures)
{
    glEnable(GL_TEXTURE_2D); // Habilita a textura
glEnable(GL_BLEND); // Habilita o blending
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Define a função de blending


// Desenha as seis faces do cubo
glBindTexture(GL_TEXTURE_2D, textures[0]); // Ativa a textura
glBegin(GL_QUADS);


    // Face frontal
    glColor3f(colors[0].r, colors[0].g, colors[0].b); 
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(points_base[0].x, points_base[0].y, points_base[0].z); // Canto inferior esquerdo
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(points_base[3].x, points_base[3].y, points_base[3].z); // Canto superior esquerdo
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(points_base[2].x, points_base[2].y, points_base[2].z); // Canto superior direito
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(points_base[1].x, points_base[1].y, points_base[1].z); // Canto inferior direito
    glEnd();

    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glBegin(GL_QUADS);
    // Face lateral 1 (0 1 5 4)
    glColor3f(colors[1].r, colors[1].g, colors[1].b);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(points_base[0].x, points_base[0].y, points_base[0].z);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(points_base[1].x, points_base[1].y, points_base[1].z);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(points_base[5].x, points_base[5].y, points_base[5].z);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(points_base[4].x, points_base[4].y, points_base[4].z);
    glEnd();

    // Face traseira (4, 5, 6, 7)
    glBindTexture(GL_TEXTURE_2D, textures[2]);
    glBegin(GL_QUADS);
    glColor3f(colors[2].r, colors[2].g, colors[2].b);
    // Invertendo coordenadas de textura na vertical
    glTexCoord2f(0.0f, 0.0f);  // Canto superior esquerdo (anteriormente 0.0f, 1.0f)
    glVertex3f(points_base[4].x, points_base[4].y, points_base[4].z); // Canto inferior esquerdo
    glTexCoord2f(1.0f, 0.0f);  // Canto superior direito (anteriormente 1.0f, 1.0f)
    glVertex3f(points_base[7].x, points_base[7].y, points_base[7].z); // Canto inferior direito
    glTexCoord2f(1.0f, 1.0f);  // Canto inferior direito (anteriormente 1.0f, 0.0f)
    glVertex3f(points_base[6].x, points_base[6].y, points_base[6].z); // Canto superior direito
    glTexCoord2f(0.0f, 1.0f);  // Canto inferior esquerdo (anteriormente 0.0f, 0.0f)
    glVertex3f(points_base[5].x, points_base[5].y, points_base[5].z); // Canto superior esquerdo
    glEnd();



    // Face lateral 2 (3 2 6 7)
    glBindTexture(GL_TEXTURE_2D, textures[3]);
    glBegin(GL_QUADS);
    glColor3f(colors[3].r, colors[3].g, colors[3].b);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(points_base[3].x, points_base[3].y, points_base[3].z);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(points_base[7].x, points_base[7].y, points_base[7].z);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(points_base[6].x, points_base[6].y, points_base[6].z);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(points_base[2].x, points_base[2].y, points_base[2].z);
    glEnd();

    // Face superior (0 3 7 4)
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

    glBindTexture(GL_TEXTURE_2D, 0); // Desativa a textura após o desenho
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
void Cube::setColors(const vector<vector<string>> &newColorMatrix)
{
    colorsMatrix = newColorMatrix;
}

void Cube::colision(int x, int y) {}

GLuint loadTexture(const char *filename)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Carrega a imagem usando stb_image
    int width, height, nrChannels;
    unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, STBI_rgb_alpha);

    if (data)
    {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        // Parâmetros de filtro
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // Libera a memória da imagem
        stbi_image_free(data);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
        return 0; // Retorna 0 para indicar falha no carregamento
    }
    return textureID;
}

GLuint Cube::mapTexture(const string &code)
{
    // Se a textura já foi carregada, retorna o ID armazenado no cache
    if (textureCache.find(code) != textureCache.end())
    {
        return textureCache[code];
    }

    GLuint textureID = 0;

    if (code[2] == 's')
    {
        textureID = loadTexture("../img/superior.png");
    }
    else if (code[2] == 'i')
    {
        textureID = loadTexture("../img/inferior.png");
    }
    else if (code[2] == 'm')
    {
        textureID = loadTexture("../img/meio.png");
    }

    textureCache[code] = textureID; // Armazena o ID da textura no cache
    return textureID;
}