// Augmented reality
// cubo.cpp
// Prof. Giovani Bernardes Vitor
// ECOI24 - 2024


#include "Objects.hpp"

#ifndef __CUBO_HPP__
#define __CUBO_HPP__

/////////////////////////////////////////////////////////////
class Cubo: public Objects
{
public:	
	Cubo();
	~Cubo();

	void draw();
	void update(int value);
	void colision(int x, int y);

private:

	int x,y;
	float theta;

	glm::vec3 points_base[4];
	glm::vec3 points[4];


private:

	void transform(void);
};

#endif
