// Elo Maluco
// cube.hpp

#include "Objects.hpp"
#include <glm/glm.hpp>
#include <vector>

#ifndef __CUBE_HPP__
#define __CUBE_HPP__

using namespace std;
using namespace glm;

class Cube: public Objects
{
public:
    Cube(const vector<vector<vec3>>& colorsMatrix);
    ~Cube();

    void draw();
    void update(int value);
    void colision(int x, int y);

private:
    int x, y, z;
    float theta;
    vec3 points_base[8];
    vec3 points[8];
    vector<vector<vec3>> colorsMatrix;  // Matriz de cores

private:
    void transform(void);
};

#endif
