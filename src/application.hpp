// Elo Maluco
// application.hpp

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <stack>
#include <GL/glut.h>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fstream>

#include <cstdlib>
#include <algorithm>

#include <random>

#include <ctime>
#include <iomanip>
#include <sstream>

#include "tinyxml2.h"
#include "Objects.hpp"
#include "cube.hpp"

#ifndef __APPLICATION_HPP__
#define __APPLICATION_HPP__

using namespace tinyxml2;
using namespace std;
using namespace glm;

/////////////////////////////////////////////////////////////
class Application
{
public:
	Application(int argc, char **argv);
	~Application(void);

	void createMenu();
	void createLoadMenu();	
	void drawMenu();
	void RandomColorMatrix();

	void draw();
	void resize(GLsizei w, GLsizei h);
	void KeyboardHandle(unsigned char key, int x, int y);
	void MouseHandle(int button, int state, int x, int y);
	void SpecialKeyHandle(int key, int x, int y);
	void update(int value, void (*func_ptr)(int));

	void processXML(const string &filename);

	void setLight();
	
	vector<string> listarArquivosXML(const string &directory); 
	void menuCallback(int value);

	int menuID;
	int submenuID;
	bool menuVisible;

	static void menuCallbackWrapper(int value);

private:
	int time;

	GLfloat xf;
	GLfloat yf;
	GLfloat win;
	GLint view_w;
	GLint view_h;

	list<Objects *> list_;
	vector<vector<string>> colorMatrix; 

	void printColorMatrix() const;

	void loadColorsFromFile(const string &filename);

	float globalRotation;

	pair<int, int> findEmptyHouse();

	void moveEmptyHouse(int direction);

	int selectedCubeIndex = -1;

	void selectCube(int index);

	void rotateRight();
	void rotateLeft();

	void drawText(float x, float y, const char *message);

	void drawFixedText();

	void updateCubeColors();

	bool isSolved();
	bool gameSolved = false;

	void saveGameStateToXML();
	bool gameSave = false;

	static Application *getInstance();

private:
	static Application *appInstance;

	void Inicializa(void);
	bool insert_object();
};

#endif
