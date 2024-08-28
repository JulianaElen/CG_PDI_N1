// Elo Maluco
// cube.hpp

#include "Objects.hpp"
#include <glm/glm.hpp>

#ifndef __CUBE_HPP__
#define __CUBE_HPP__

class Cube: public Objects
{
public:
    Cube();
    ~Cube();

    void draw();
    void update(int value);
    void colision(int x, int y);

private:
    int x, y, z;
    float theta;
    glm::vec3 points_base[8];
    glm::vec3 points[8];

private:
    void transform(void);
};

#endif
