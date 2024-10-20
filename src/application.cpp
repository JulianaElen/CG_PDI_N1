// Elo Maluco
// application.cpp

#include <stdlib.h>
#include <stdio.h>
#include "application.hpp"

// Variavel estática do MEnu
Application *globalAppInstance = nullptr;
// Definição da variável estática
Application *Application::appInstance = nullptr;

///////////////////////////////////////////////////////////////////////
// Application Class (construtor)
Application::Application(int argc, char **argv)
{

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);

    glutCreateWindow("ELO MALUCO");
    Inicializa();

    menuVisible = true; // Suponha que o menu esteja visível inicialmente
    // Cria o menu
    createMenu();

    processXML("../data/EloMaluco_estadoAtual_teste01.xml");

    insert_object();
}

//---------------------------------------------------------------------
// Destrutor
Application::~Application()
{
}

//---------------------------------------------------------------------
void Application::Inicializa(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // cor da janela
    xf = 50.0f;
    yf = 50.0f;
    win = 250.0f;
    time = 0;
}

//---------------------------------------------------------------------
// void Application::DisplayFunc()
//{
//	glutDisplayFunc(Application::Desenha);
//}
//_______________MENU_____________________________________________________
// Função para criar o menu no GLUT
void Application::createMenu()
{
    menuID = glutCreateMenu(menuCallbackWrapper); // Cria o menu e associa o callback
    glutAddMenuEntry("Iniciar aleatoriamente e jogar manualmente", 1);
    glutAddMenuEntry("Iniciar por arquivo XML e jogar manualmente", 2);
    glutAddMenuEntry("Iniciar por arquivo XML e executar passo a passo", 3);
    glutAddMenuEntry("Salvar e sair", 4);
    glutAddMenuEntry("Sair", 5);

    glutAttachMenu(GLUT_RIGHT_BUTTON); // Anexa o menu ao botão direito do mouse
}

// Função de callback do menu GLUT
void Application::menuCallback(int value)
{
    switch (value)
    {
    case 1:
        // Iniciar o jogo manualmente com estado aleatório
        // Função para iniciar o jogo com estado aleatório (a ser implementada)
        std::cout << "Iniciar aleatoriamente e jogar manualmente" << std::endl;
        // Exemplo: iniciarJogoAleatorio();
        break;
    case 2:
        // Carregar estado do XML e jogar manualmente
        processXML("../data/EloMaluco_estadoAtual_teste01.xml");
        std::cout << "Carregar estado do XML e jogar manualmente" << std::endl;
        break;
    case 3:
        // Iniciar jogo por arquivo XML e executar o passo a passo
        std::cout << "Executar o passo a passo do arquivo XML" << std::endl;
        // Exemplo: iniciarJogoPorXMLPassoAPasso();
        break;
    case 4:
        // Salvar o estado atual do Jogo e sai 
        processXML("../data/EloMaluco_estadoAtual_teste01.xml");
        std::cout << "Salvando o estado atual do jogo" << std::endl;
        break;
    case 5:
        // Sair do Jogo
        exit(0);
        break;
    default:
        break;
    }
    glutPostRedisplay(); // Re-desenha a tela após o menu ser usado
}

void Application::menuCallbackWrapper(int option)
{
    globalAppInstance->menuCallback(option);
}

// função para desenhar o menu na tela
void Application::drawMenu()
{
    // Salva as configurações de projeção e visão
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    // Define a perspectiva da câmera especifica para o texto
    glOrtho(0, view_w, 0, view_h, -1, 1); // Projeção ortográfica
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Define a cor do texto
    glColor3f(1.0f, 0.0f, 0.0f); // Cor preta para o texto

    // Desenha esses texto na tela, (Não precisa porque no Menu poup já aparece)
    // drawText(10, view_h - 30, "Menu:");
    // drawText(10, view_h - 60, "1. Iniciar aleatoriamente e jogar manualmente");
    // drawText(10, view_h - 90, "2. Iniciar por arquivo XML e jogar manualmente");
    // drawText(10, view_h - 120, "3. Iniciar por arquivo XML e executar passo a passo");
    // drawText(10, view_h - 150, "4. Sair");

    // Restaura as configurações originais
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}
//____________________________________FIM do MENU___________________________________________
//---------------------------------------------------------------------
// imprimi a matriz das cores no console para conferir se está tudo correto
void Application::printColorMatrix() const
{
    std::cout << "Color Matrix:" << std::endl;
    for (size_t i = 0; i < colorMatrix.size(); ++i)
    {
        std::cout << "Row " << i << ": ";
        for (size_t j = 0; j < colorMatrix[i].size(); ++j)
        {
            std::cout << colorMatrix[i][j] << " "; // Imprime a letra
        }
        std::cout << std::endl;
    }
}

//---------------------------------------------------------------------
// Leitura do arquivo xml e processamento dos dados sobre as cores contidos nele. Esses dados sao  inseridoas na matriz de cores
void Application::processXML(const string &filename)
{

    XMLDocument doc;

    // verificação de carregamento do arquivo
    if (doc.LoadFile(filename.c_str()) != XML_SUCCESS)
    {
        cerr << "Erro ao carregar o arquivo XML. Verifique o caminho." << endl;
        return;
    }

    // Verifica se o xml contem o elemento raiz
    XMLElement *root = doc.RootElement();
    if (root == nullptr)
    {
        cerr << "O elemento raiz não foi encontrado." << endl;
        return;
    }

    // Verifica se existem os elemntos dentro da raiz
    XMLElement *estadoAtual = root->FirstChildElement("EstadoAtual");
    if (estadoAtual == nullptr)
    {
        cerr << "Nenhum elemento encontrado." << endl;
        return;
    }

    colorMatrix.clear();

    // percorre cada row e col's dentro dela. Armazena seus respctivos codigos (utilizando o mapeamento) dentro da nossa matriz de cores
    for (XMLElement *row = estadoAtual->FirstChildElement("row"); row != nullptr; row = row->NextSiblingElement("row"))
    {
        vector<string> colorRow;
        for (XMLElement *col = row->FirstChildElement("col"); col != nullptr; col = col->NextSiblingElement("col"))
        {
            const char *text = col->GetText();
            if (text)
            {
                string code(text);
                colorRow.push_back(code); // Adiciona o código direto
            }
        }
        colorMatrix.push_back(colorRow);
    }
    printColorMatrix();
}

//---------------------------------------------------------------------
void Application::draw()
{
    glEnable(GL_DEPTH_TEST);                            // arruma a profundidade
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Limpa a janela e o Depth Buffer

    glLoadIdentity();

    setLight();

    glTranslatef(7.5f, 0.0f, 7.5f);

    glRotatef(globalRotation, 0.0f, 1.0f, 0.0f); // Rotação global

    glTranslatef(-7.5f, 0.0f, -7.5f);

    // Desenha os cubos
    for (auto obj : list_)
    {
        obj->draw();
    }

    if (menuVisible)
    {
        drawMenu();
    }
    // Desenhado alguns textos na tela
    // drawFixedText(); //TIrei o Desenho do TExto somente para aparecer somente o menu

    // mostrando
    glutSwapBuffers();
}
//---------------------------------------------------------------------
// Função para desenhar o texto fixo na tela (Não vai girar junto com o Elo)
void Application::drawFixedText()
{

    if (gameSolved)
    {
        drawText(-20.0f, -15.f, "O jogo esta resolvido!"); // Adicione a mensagem na tela
    }

    if (gameSave)
    {
        drawText(-20.0f, -25.f, "O jogo esta salvo!"); // Adicione a mensagem na tela
    }
    // Salva as configurações de projeção e visão
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    // Define a perspectiva da câmera especifica para o texto
    gluPerspective(55, (GLdouble)view_w / view_h, 1, 500);
    gluLookAt(0.0, 0.0, 150.0, 7.5f, 30.0f, 7.5f, 0.0, 1.0, 0.0);

    // Salva a configuração de modelo e visão
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Define a cor do texto
    glColor3f(1.0f, 1.0f, 1.0f); // Cor branca para o texto

    // Desenhando umas instruções aleatorias que vão ser colocadas no menu depois
    //  drawText(-60.0f, -15.f, "Instrucoes para jogar: ");
    // drawText(-60.0f, -22.f, "SETAS rotacionam o Elo Maluco;");
    // drawText(-60.0f, -27.f, "Setas para cima e para baixo movem a casa vazia;");
    // drawText(-60.0f, -32.f, "F1 e F2 selecionam o cubo;");
    // drawText(-60.0f, -37.f, "CRTL + SETAS rotacionam o cubo selecionado;");

    // Vericiando qual cubo foi selecionado, para mostrar uma seta informativa
    if (selectedCubeIndex != -1)
    {
        float textPosX = -15.0f; // Posição no eixo x da seta
        float textPosY = 0.0f;

        // Definindo cada posicão em y de acordo com o cubo selecionado
        if (selectedCubeIndex == 0)
        {
            textPosY = 52.5f;
        }
        else if (selectedCubeIndex == 1)
        {
            textPosY = 37.5f;
        }
        else if (selectedCubeIndex == 2)
        {
            textPosY = 22.5f;
        }
        else
        {
            textPosY = 7.5f;
        }

        // Desenha a seta na posição do cubo selecionado
        drawText(textPosX, textPosY, "->");
    }

    // Restaura as configurações originais
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}
//---------------------------------------------------------------------
// Função auxiliar para desenhar texto na tela
void Application::drawText(float x, float y, const char *message)
{
    glRasterPos2f(x, y); // Define a posição do texto no espaço 2D
    for (const char *c = message; *c != '\0'; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c); // Desenha cada caractere
    }
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
    gluPerspective(60, (GLdouble)view_w / view_h, 1, 500);
    gluLookAt(0.0, 0.0, 150.0,   // Posição da câmera
              7.5f, 30.0f, 7.5f, // Ponto para onde a câmera olha
              0.0, 1.0, 0.0);    // Vetor "up"

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

//---------------------------------------------------------------------
void Application::KeyboardHandle(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'R':
    case 'r': // muda a cor corrente para vermelho
        glColor3f(1.0f, 0.0f, 0.0f);
        break;
    case 'G':
    case 'g': // muda a cor corrente para verde
        glColor3f(0.0f, 1.0f, 0.0f);
        break;
    case 'B':
    case 'b': // muda a cor corrente para azul
        glColor3f(0.0f, 0.0f, 1.0f);
        time++;
        for (list<Objects *>::const_iterator it = list_.begin(); it != list_.end(); ++it)
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
// Função que gerencia os clicks do mouse
void Application::MouseHandle(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
    }
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
    }
}
//---------------------------------------------------------------------
void Application::SpecialKeyHandle(int key, int x, int y)
{

    // Verifica se Ctrl está pressionado
    bool ctrlPressed = (glutGetModifiers() & GLUT_ACTIVE_CTRL);

    switch (key)
    {
    case GLUT_KEY_LEFT:
        if (ctrlPressed)
        {
            // Ação quando Ctrl + Seta Esquerda é pressionado
            rotateLeft();
        }
        else
        {
            // Ação quando apenas Seta Esquerda é pressionada

            globalRotation -= 5.0f; // Rotaciona 5 graus no sentido anti-horário
        }
        break;

    case GLUT_KEY_RIGHT:
        if (ctrlPressed)
        {
            // Ação quando Ctrl + Seta Direita é pressionado
            rotateRight();
        }
        else
        {
            // Ação quando apenas Seta Direita é pressionada
            globalRotation += 5.0f; // Rotaciona 5 graus no sentido horário
        }
        break;
    case GLUT_KEY_UP: // Mover para cima a casa vazia
        moveEmptyHouse(-1);
        break;
    case GLUT_KEY_DOWN: // Mover para baixo a casa vazia
        moveEmptyHouse(1);
        break;
    case GLUT_KEY_F1: // Seleciona o cubo 1
        selectCube(0);
        break;
    case GLUT_KEY_F2: // Seleciona o cubo 2
        selectCube(3);
        break;
    case GLUT_KEY_INSERT:     // Tecla para salvar o estado do jogo
        saveGameStateToXML(); // Salva no arquivo 'estado_jogo.xml'
        break;
    }

    glutPostRedisplay(); // Re-desenhar a cena
}

//---------------------------------------------------------------------
void Application::update(int value, void (*func_ptr)(int))
{
    glutPostRedisplay();
    glutTimerFunc(30, func_ptr, time);
}

//---------------------------------------------------------------------
// Conversão de cor, de acordo com o modelo
vec3 colorToVec3(const Color &color)
{
    return glm::vec3(color.r, color.g, color.b);
}

//---------------------------------------------------------------------
bool Application::insert_object()
{
    Cube *cube = new Cube(colorMatrix);
    list_.push_back(cube);
    return true;
}
//---------------------------------------------------------------------

// Encontra a casa vazia e retorna um par de inteiros (linha e coluna dessa casa)
pair<int, int> Application::findEmptyHouse()
{
    for (size_t i = 0; i < colorMatrix.size(); ++i) // Percorre as linhas
    {
        for (size_t j = 0; j < colorMatrix[i].size(); ++j) // Percorre as colunas
        {
            if (colorMatrix[i][j] == "vzo") // Se a casa for vazia (cinza)
            {
                return {static_cast<int>(i), static_cast<int>(j)};
            }
        }
    }
    return {-1, -1}; // Retorna -1, -1 se não encontrar
}
//---------------------------------------------------------------------
// Movimentação da casa vazia
void Application::moveEmptyHouse(int direction)
{
    pair<int, int> emptyHousePosition = findEmptyHouse(); // Primeiro localiza a casa vaxia
    int emptyRow = emptyHousePosition.first;
    int emptyCol = emptyHousePosition.second;

    // Verifica se a casa vazia foi encontrada
    if (emptyRow == -1)
        // Adicionar um possivel tratamento de erro
        return;

    // Calcula a nova linha, para onde o vazio deve ser movido
    int newRow = emptyRow + direction;

    // Verifica se o novo índice está dentro da matriz
    if (newRow >= 0 && newRow < colorMatrix.size())
    {
        // Troca as cores
        swap(colorMatrix[emptyRow][emptyCol], colorMatrix[newRow][emptyCol]);

        printColorMatrix();

        // Atualiza a matriz de cores de geração dos cubos
        updateCubeColors();

        isSolved();

        gameSave = false;

        glutPostRedisplay();
    }
}
//---------------------------------------------------------------------
void Application::selectCube(int index)
{
    selectedCubeIndex = index; // Define o índice do cubo selecionado
}
//---------------------------------------------------------------------
// Rotação para a esquerda
void Application::rotateLeft()
{
    if (selectedCubeIndex != -1) // Verifica se o cubo foi selecionado
    {
        auto &matrix = colorMatrix[selectedCubeIndex]; // Passa a matriz/linha do cubo selecionado por referencia
        string aux = matrix[0];                        // Guarda a primeira cor (pois ela vai virar a ultima devido ao deslocamento)

        for (int i = 0; i < matrix.size() - 1; ++i)
        {
            matrix[i] = matrix[i + 1]; // Desloca para a esquerda todos as cores
        }

        matrix.back() = aux;                       // Coloca a primeira cor no final
        cout << "Cubo girado à esquerda." << endl; // Mensagem para conferir no terminal

        printColorMatrix();

        // Atualiza a matriz de cores de todos os cubos
        updateCubeColors();

        isSolved();

        gameSave = false;

        glutPostRedisplay();
    }
}
//---------------------------------------------------------------------
// Rotação para a direita
void Application::rotateRight()
{
    if (selectedCubeIndex != -1)
    {
        auto &matrix = colorMatrix[selectedCubeIndex]; // Paassa a matriz/linha do cubo selecionado por referencia
        string aux = matrix.back();                    // Guarda a última cor, pois ela vai virar a primeira

        for (int i = matrix.size() - 1; i > 0; --i)
        {
            matrix[i] = matrix[i - 1]; // Desloca para a direita
        }

        matrix[0] = aux;                          // Coloca a última cor no início
        cout << "Cubo girado à direita." << endl; // Mensagem no terminal

        printColorMatrix();

        // Atualiza a matriz de cores de todos os cubos
        updateCubeColors();

        isSolved();

        gameSave = false;

        glutPostRedisplay();
    }
}
//---------------------------------------------------------------------
// Atualização da matriz de cores nas alterações especificas
void Application::updateCubeColors()
{
    for (Objects *obj : list_)
    {
        Cube *cube = dynamic_cast<Cube *>(obj); // conversão
        if (cube)
        {
            cube->setColors(colorMatrix); // Atualiza se for um Cube
        }
    }
}

void Application::setLight()
{
    // Habilitar iluminação
    glEnable(GL_LIGHTING);

    // Propriedades da luz
    GLfloat light_position[] = {50.0f, 50.0f, 50.0f, 1.0f}; // Luz em coordenadas homogêneas

    GLfloat light_ambient[] = {0.3f, 0.3f, 0.3f, 1.0f};  // Luz ambiente
    GLfloat light_diffuse[] = {0.9f, 0.9f, 0.9f, 1.0f};  // Luz difusa
    GLfloat light_specular[] = {0.5f, 0.5f, 0.5f, 1.0f}; // Luz especular

    // Configurar luz
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    // Habilitar a luz
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);

    // Configurar modelo de iluminação
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    GLfloat lmodel_ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
}

bool Application::isSolved()
{

    // Verifica se cada coluna tem apenas uma cor, ignorando "vzo"
    for (size_t col = 0; col < colorMatrix[0].size(); ++col)
    {
        string firstColor;
        bool foundColor = false;

        // Verifica cada linha da coluna
        for (size_t row = 0; row < colorMatrix.size(); ++row)
        {
            string currentColor = colorMatrix[row][col];

            // Ignora "vzo"
            if (currentColor == "vzo")
                continue;

            // Se ainda não encontrou uma cor, define a primeira cor
            if (!foundColor)
            {
                firstColor = currentColor.substr(0, 2);
                foundColor = true;
            }
            else
            {
                string colorToCheck = currentColor.substr(0, 2);
                if (colorToCheck != firstColor)
                {
                    gameSolved = false; // O jogo não está resolvido
                    return false;
                }
            }
        }
    }

    // Verifica as letras da terceira posição para cada linha
    if (colorMatrix[0][0][2] != 's')
    {
        return false;
    }

    for (size_t row = 1; row <= 2; ++row)
    {
        if (colorMatrix[row][0][2] != 'm')
        {
            return false;
        }
    }

    if (colorMatrix[3][0][2] != 'i')
    {
        return false;
    }

    gameSolved = true; // O jogo está resolvido
    cout << "Jogo resolvido!" << endl;
    return true;
}

void Application::saveGameStateToXML()
{
    // Cria o diretório "data" se não existir
    std::system("mkdir -p data");

    // Obtém a data e hora atuais
    std::time_t now = std::time(nullptr);
    std::tm *now_tm = std::localtime(&now);

    // Formata a data e a hora como string
    std::ostringstream oss;
    oss << "../data/estado_jogo_";
    oss << (now_tm->tm_year + 1900) << "-"                                  // Ano
        << std::setw(2) << std::setfill('0') << (now_tm->tm_mon + 1) << "-" // Mês
        << std::setw(2) << std::setfill('0') << now_tm->tm_mday << "_"      // Dia
        << std::setw(2) << std::setfill('0') << now_tm->tm_hour << "-"      // Hora
        << std::setw(2) << std::setfill('0') << now_tm->tm_min << "-"       // Minuto
        << std::setw(2) << std::setfill('0') << now_tm->tm_sec;             // Segundo

    // Nome do arquivo com base na data e hora
    std::string filepath = oss.str() + ".xml";

    // Salva o arquivo XML
    std::ofstream outFile(filepath);
    if (outFile.is_open())
    {
        outFile << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n";
        outFile << "<EloMaluco>\n";
        outFile << "  <EstadoAtual>\n";

        for (int i = 0; i < 4; i++)
        {
            outFile << "      <row>\n";
            for (int j = 0; j < 4; j++)
            {
                outFile << "          <col>" << colorMatrix[i][j] << "</col>\n";
            }
            outFile << "      </row>\n";
        }

        outFile << "  </EstadoAtual>\n";
        outFile << "</EloMaluco>\n";

        outFile.close();
    }

    cout << "Estado do jogo Salvo!" << endl;
    gameSave = true;
}
