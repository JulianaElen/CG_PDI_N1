// Elo Maluco
// application.cpp

#include <stdlib.h>
#include <stdio.h>
#include "application.hpp"

///////////////////////////////////////////////////////////////////////
// Application Class
Application::Application(int argc, char** argv)
{
	
	glutInit(&argc,argv);
     	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
     	glutInitWindowSize(500,500);
   	glutInitWindowPosition(100,100);
   	glutCreateWindow("ELO MALUCO");
	Inicializa();
    insert_object();

    stack<string> colorStack;
    processXML("../data/EloMaluco_estadoAtual_exemplo.xml", colorStack);
    printColorStack(colorStack);
}

//---------------------------------------------------------------------
Application::~Application()
{
}
//---------------------------------------------------------------------
void Application::Inicializa (void)
{   
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //cor da janela para pretoé
    xf=50.0f;
    yf=50.0f;
    win=250.0f;
    time = 0;
}
//---------------------------------------------------------------------
//void Application::DisplayFunc()
//{
//	glutDisplayFunc(Application::Desenha);
//}
//---------------------------------------------------------------------

//mapeanmento das cores de acordo com o codigo
string Application::mapColor(const string& code) {
    if (code == "vms" || code == "vmm" || code == "vmi" ) return "vm";
    if (code == "vds" || code == "vdm" || code == "vdi" ) return "vd";
    if (code == "ams" || code == "amm" || code == "ami" ) return "am";
    if (code == "brs" || code == "brm" || code == "bri" ) return "br";
    if (code == "vzo") return "vzo";
    return "desconhecido";
}

// processa o xml e coloca as cores em uma pilha
void Application::processXML(const string& filename, stack<string>& colorStack) {
    XMLDocument doc;
    if (doc.LoadFile(filename.c_str()) != XML_SUCCESS) {
        cerr << "Erro ao carregar o arquivo XML." << endl;
        return;
    }

    XMLElement* root = doc.RootElement();
    if (root == nullptr) {
        cerr << "Elemento raiz não encontrado." << endl;
        return;
    }

    XMLElement* estadoAtual = root->FirstChildElement("EstadoAtual");
    if (estadoAtual == nullptr) {
        cerr << "Elemento EstadoAtual não encontrado." << endl;
        return;
    }

    for (XMLElement* row = estadoAtual->FirstChildElement("row"); row != nullptr; row = row->NextSiblingElement("row")) {
        for (XMLElement* col = row->FirstChildElement("col"); col != nullptr; col = col->NextSiblingElement("col")) {
            const char* text = col->GetText();
            if (text) {
                string code(text);
                colorStack.push(mapColor(code));
            }
        }
    }
}

//imprimindo a pilha para conferir se esta certo (apagar depois)
void Application::printColorStack(const stack<string>& colorStack) {
    stack<string> tempStack = colorStack;

    cout << "Conteúdo da pilha:" << endl;

    while (!tempStack.empty()) {
        cout << tempStack.top() << endl;
        tempStack.pop();
    }
}

void Application::draw()
{
    glEnable(GL_DEPTH_TEST); // arruma a profundidade

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Limpa a janela e o Depth Buffer
    glLoadIdentity();

    // Desenha os eixos
    glLineWidth(2.0f);
    glBegin(GL_LINES);

    glColor3f(1.0f, 0.0f, 0.0f); //vermelho
    glVertex3f(-100.0f, 0.0f, 0.0f);
    glVertex3f(100.0f, 0.0f, 0.0f);

    glColor3f(0.0f, 1.0f, 0.0f); //verde
    glVertex3f(0.0f, -100.0f, 0.0f);
    glVertex3f(0.0f, 100.0f, 0.0f);

    glColor3f(0.0f, 0.0f, 1.0f); //azual
    glVertex3f(0.0f, 0.0f, -100.0f);
    glVertex3f(0.0f, 0.0f, 100.0f);

    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);

    // Desenha os cubos
    for (auto obj : list_) {
        obj->draw();
    }

    // mostrando
    glutSwapBuffers();
}


//---------------------------------------------------------------------
void Application::resize(GLsizei w, GLsizei h)
{ 
    // Especifica as dimensões da Viewport
    glViewport(0, 0, w, h);
    view_w = w;
    view_h = h;                   

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // Aumente a distância da câmera para ter uma visão mais ampla
    gluPerspective(60, (GLdouble)view_w/view_h, 1, 500); // Ajuste o "far plane" se necessário

    // Ajuste a posição da câmera para garantir que ela esteja olhando para os cubos
    gluLookAt(100.0, 100.0, 100.0,  // Posição da câmera
          0.0, 0.0, 0.0,    // Ponto para onde a câmera olha
          0.0, 1.0, 0.0);   // Vetor "up"

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


//---------------------------------------------------------------------
void Application::KeyboardHandle(unsigned char key, int x, int y)
{
    switch (key) {
            case 'R': 
            case 'r':// muda a cor corrente para vermelho
                     glColor3f(1.0f, 0.0f, 0.0f);
                     break;
            case 'G':
            case 'g':// muda a cor corrente para verde
                     glColor3f(0.0f, 1.0f, 0.0f);
                     break;
            case 'B':
            case 'b':// muda a cor corrente para azul
                     glColor3f(0.0f, 0.0f, 1.0f);
			time++;
			for(list<Objects*>::const_iterator it = list_.begin(); it != list_.end(	) ;  ++it)
      			{
    	   			(*it)->update(time);
      			}
                     break;
	    case 27: // tecla Esc (encerra o programa)
		     exit(0);
		     break;
    }
    
}
        
//---------------------------------------------------------------------
void Application::MouseHandle(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
         if (state == GLUT_DOWN) {
                  // Troca o tamanho do retângulo, que vai do centro da 
                  // janela até a posição onde o usuário clicou com o mouse
                  //xf = ( (2 * win * x) / view_w) - win;
                  //yf = ( ( (2 * win) * (y-view_h) ) / -view_h) - win;
         }
    
}


//---------------------------------------------------------------------
void Application::SpecialKeyHandle(int key, int x, int y)
{
    if(key == GLUT_KEY_UP) {
           //win -= 20;
           //glMatrixMode(GL_PROJECTION);
           //glLoadIdentity();
           //gluOrtho2D (-win, win, -win, win);
	   insert_object();
    }
    if(key == GLUT_KEY_DOWN) {
           //win += 20;
           //glMatrixMode(GL_PROJECTION);
           //glLoadIdentity();
           //gluOrtho2D (-win, win, -win, win);
    }
    
}
//---------------------------------------------------------------------
void Application::update(int value, void (*func_ptr)(int))
{	
	glutPostRedisplay();
	glutTimerFunc(30,func_ptr,time);

}
//---------------------------------------------------------------------
bool Application::insert_object(void) {
    Cube* cube = new Cube();
    list_.push_back(cube);
    return true;
}

