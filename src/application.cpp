// Elo Maluco
// application.cpp

#include <stdlib.h>
#include <stdio.h>
#include "application.hpp"

///////////////////////////////////////////////////////////////////////
// Application Class (construtor)
Application::Application(int argc, char** argv)
{
	
	glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(500,500);
   	glutInitWindowPosition(100,100);

   	glutCreateWindow("ELO MALUCO");
	Inicializa();

    processXML("../data/EloMaluco_estadoAtual_teste01.xml");

    insert_object();
    
}

//---------------------------------------------------------------------
//Destrutor 
Application::~Application()
{
}

//---------------------------------------------------------------------
void Application::Inicializa (void)
{   
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //cor da janela 
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
//faz o mapeamento das cores no xml para o formato rgb que precisamos
Color Application::mapColor(const string& code) {
    if (code == "vms" || code == "vmi" || code == "vmm" ) return {1.0f, 0.0f, 0.0f}; // vrmelho
    if (code == "vds" || code == "vdi" || code == "vdm" || code == "vrs" || code == "vri" || code == "vrm"  ) return {0.0f, 1.0f, 0.0f}; // Verde
    if (code == "ams" || code == "ami" || code == "amm" ) return {1.0f, 1.0f, 0.0f}; // Amarelo
    if (code == "brs" || code == "bri" || code == "brm" ) return {1.0f, 1.0f, 1.0f}; // branco
    if (code == "vzo") return {0.7f, 0.7f, 0.7f}; // cinza
    return {0.0f, 0.0f, 0.0f}; // preto
}

//---------------------------------------------------------------------
//imprimi a matriz das cores no console para conferir se está tudo correto
void Application::printColorMatrix() const {
    std::cout << "Color Matrix:" << std::endl;
    for (size_t i = 0; i < colorMatrix.size(); ++i) {
        std::cout << "Row " << i << ": ";
        for (size_t j = 0; j < colorMatrix[i].size(); ++j) {
            const glm::vec3& color = colorMatrix[i][j];
            std::cout << "(" << color.r << ", " << color.g << ", " << color.b << ") ";
        }
        std::cout << std::endl;
    }
}

//---------------------------------------------------------------------
// Leitura do arquivo xml e processamento dos dados sobre as cores contidos nele. Esses dados sao  inseridoas na matriz de cores 
void Application::processXML(const string& filename) {

    XMLDocument doc; 

    //verificação de carregamento do arquivo 
    if (doc.LoadFile(filename.c_str()) != XML_SUCCESS) {
        cerr << "Erro ao carregar o arquivo XML. Verifique o caminho." << endl;
        return;
    }

    // Verifica se o xml contem o elemento raiz
    XMLElement* root = doc.RootElement();
    if (root == nullptr) {
        cerr << "O elemento raiz não foi encontrado." << endl;
        return;
    }

    // Verifica se existem os elemntos dentro da raiz
    XMLElement* estadoAtual = root->FirstChildElement("EstadoAtual");
    if (estadoAtual == nullptr) {
        cerr << "Nenhum elemento encontrado." << endl;
        return;
    }

    colorMatrix.clear(); 

    //percorre cada row e col's dentro dela. Armazena seus respctivos codigos (utilizando o mapeamento) dentro da nossa matriz de cores
    for (XMLElement* row = estadoAtual->FirstChildElement("row"); row != nullptr; row = row->NextSiblingElement("row")) {
        vector<vec3> colorRow;
        for (XMLElement* col = row->FirstChildElement("col"); col != nullptr; col = col->NextSiblingElement("col")) {
            const char* text = col->GetText();
            if (text) {
                string code(text);
                Color color = mapColor(code);
                colorRow.push_back(vec3(color.r, color.g, color.b)); // Conversão das cores para vec3
            }
        }
        colorMatrix.push_back(colorRow);
    }
    printColorMatrix();
}

//---------------------------------------------------------------------
void Application::draw()
{
    glEnable(GL_DEPTH_TEST); // arruma a profundidade

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Limpa a janela e o Depth Buffer
    glLoadIdentity();

    glTranslatef(7.5f, 0.0f, 7.5f);

    glRotatef(globalRotation, 0.0f, 1.0f, 0.0f);

    glTranslatef(-7.5f, 0.0f, -7.5f);

    // Desenha os eixos x, y e z
    //glLineWidth(2.0f);
    //glBegin(GL_LINES);

    //glColor3f(1.0f, 0.0f, 0.0f); //vermelho
    //glVertex3f(-100.0f, 0.0f, 0.0f);
    //glVertex3f(100.0f, 0.0f, 0.0f);

    //glColor3f(0.0f, 1.0f, 0.0f); //verde
    //glVertex3f(0.0f, -100.0f, 0.0f);
    //glVertex3f(0.0f, 100.0f, 0.0f);

    //glColor3f(0.0f, 0.0f, 1.0f); //azual
    //glVertex3f(0.0f, 0.0f, -100.0f);
    //glVertex3f(0.0f, 0.0f, 100.0f);

    //glEnd();

    //glColor3f(1.0f, 1.0f, 1.0f);

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
    
    // definição da pespectiva da câmera
    gluPerspective(60, (GLdouble)view_w/view_h, 1, 500);
    gluLookAt(0.0, 0.0, 150.0,  // Posição da câmera
          7.5f, 30.0f, 7.5f,    // Ponto para onde a câmera olha
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
    switch (key)
    {
    case GLUT_KEY_LEFT:
        globalRotation -= 5.0f;  // Rotaciona 5 graus no sentido anti-horário
        break;
    case GLUT_KEY_RIGHT:
        globalRotation += 5.0f;  // Rotaciona 5 graus no sentido horário
        break;
    }

    glutPostRedisplay();  // Re-desenhar a cena
}

//---------------------------------------------------------------------
void Application::update(int value, void (*func_ptr)(int))
{	
	glutPostRedisplay();
	glutTimerFunc(30,func_ptr,time);

}

//---------------------------------------------------------------------
//Conversão de cor, de acordo com o modelo 
vec3 colorToVec3(const Color& color) {
    return glm::vec3(color.r, color.g, color.b);
}

//---------------------------------------------------------------------
bool Application::insert_object() {
    Cube* cube = new Cube(colorMatrix);
    list_.push_back(cube);
    return true;
}