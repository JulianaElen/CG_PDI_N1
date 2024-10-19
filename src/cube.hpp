// Elo Maluco
// cube.hpp

#include "Objects.hpp"
#include <glm/glm.hpp>
#include <vector>
#include <unordered_map>

#ifndef __CUBE_HPP__
#define __CUBE_HPP__

using namespace std;
using namespace glm;
struct Color
	{
		float r, g, b;
	};
class Cube: public Objects
{
public:
    Cube(const vector<vector<string>>& colorsMatrix);
    ~Cube();

    void draw();
    void drawCube(const vector<Color>& colors, const vector<GLuint>& textures);
    void update(int value);
    void colision(int x, int y);

     void setColors(const vector<vector<string>>& newColorsMatrix);
     Color mapColor(const string& code);

GLuint mapTexture(const string& code) ;
private:
    int x, y, z;
    float theta;
    vec3 points_base[8];
    vec3 points[8];
    vector<vector<string>> colorsMatrix;  // Matriz de cores

   std::unordered_map<std::string, GLuint> textureCache;

private:
    void transform(void);
};

#endif
