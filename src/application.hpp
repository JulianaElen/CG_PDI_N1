// Elo Maluco
// application.hpp

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <stack>

#include <GL/glut.h>

#include "tinyxml2.h"
#include "Objects.hpp"
#include "cube.hpp"

#ifndef __APPLICATION_HPP__
#define __APPLICATION_HPP__

using namespace tinyxml2;
using namespace std;
using namespace glm;

/////////////////////////////////////////////////////////////
struct Color
	{
		float r, g, b;
	};
class Application
{
public:
	Application(int argc, char** argv);
	~Application(void);

	void draw();
	void resize(GLsizei w, GLsizei h);
	void KeyboardHandle(unsigned char key, int x, int y);
	void MouseHandle(int button, int state, int x, int y);
	void SpecialKeyHandle(int key, int x, int y);
	void update(int value, void (*func_ptr)(int));

	Color mapColor(const string& code);
	void processXML(const string& filename);

private:

	int time;
	
	GLfloat xf;
	GLfloat yf;
	GLfloat win;
	GLint view_w;
	GLint view_h;

	list<Objects*> list_;
	vector<vector<vec3>> colorMatrix; // Matriz de cores

	void printColorMatrix() const;

    void loadColorsFromFile(const string& filename);


	float globalRotation;

private:

	void Inicializa (void);
	bool insert_object();
	
};

#endif
