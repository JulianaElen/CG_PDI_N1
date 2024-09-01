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

/////////////////////////////////////////////////////////////
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

	string mapColor(const string& code); 
	void processXML(const string& filename, stack<string>& colorStack);
	void printColorStack(const std::stack<std::string>& colorStack);

private:

	int time;
	
	GLfloat xf;
	GLfloat yf;
	GLfloat win;
	GLint view_w;
	GLint view_h;

	list<Objects*> list_;

private:

	void Inicializa (void);
	bool insert_object(void);
	
};

#endif
