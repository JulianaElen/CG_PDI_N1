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
    // Variaável global do MENU
    globalAppInstance = this; // Inicializa o ponteiro global com a instância atual

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);

    glutCreateWindow("ELO MALUCO");
    Inicializa();

    // Cria o menu
    createMenu();

    processXML("../data/EloMaluco_estadoAtual_teste01.xml");
    insert_object();
}

//---------------------------------------------------------------------
// Destrutor
Application::~Application() {}

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
//_______________MENU_____________________________________________________________________
// Função para criar o menu no GLUT

void Application::createLoadMenu() // Submenu1
{
    vector<string> arquivos = listarArquivosXML("../data"); // Lista arquivos no diretório
    int submenuID = glutCreateMenu(menuCallbackWrapper);    // Cria submenu

    for (size_t i = 0; i < arquivos.size(); ++i)
    {
        glutAddMenuEntry(arquivos[i].c_str(), i + 6); // diciona arquivos ao submenu
    }
    glutAddSubMenu("Voltar o Menu principal", menuID);
    glutAttachMenu(GLUT_RIGHT_BUTTON); // Anexa o submenu ao botão direito do mouse
}

void Application::createMenu() // submenu2
{
    menuID = glutCreateMenu(menuCallbackWrapper); // Cria o menu e associa o callback
    glutAddMenuEntry("Iniciar aleatoriamente e jogar manualmente", 1);
    glutAddMenuEntry("Iniciar por arquivo XML e jogar manualmente", 2);
    glutAddMenuEntry("Iniciar por arquivo XML e executar passo a passo", 3);
    glutAddMenuEntry("Salvar", 4);
    glutAddMenuEntry("Sair", 5);

    glutAddSubMenu("Carregar XML", submenuID); // Adiciona o submenu ao menu principal

    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// Função de callback do menu GLUT
void Application::menuCallback(int value)
{
    switch (value)
    {
    case 1:
        // Iniciar o jogo  com estado aleatório
        cout << "Iniciar aleatoriamente e jogar manualmente" << endl;
        RandomColorMatrix();
        break;
    case 2:
        cout << "Clique com o botão direito do mouse para selecionar o estado salvo" << endl;
        // timer para atrasar a seleção do arquivo XML
        glutTimerFunc(100, [](int value)
                      { globalAppInstance->createLoadMenu(); }, 0);
        break;
    case 3:
        // Iniciar jogo por arquivo XML e executar o passo a passo
        cout << "Executar o passo a passo do arquivo XML" << endl;
        break;
    case 4:
        // Salvar o estado atual do Jogo e sai
        saveGameStateToXML();
        cout << "Salvando o estado atual do jogo" << endl;
        break;
    case 5:
        // Sair do Jogo
        exit(0);
        break;
    default:
    {
        int index = value - 6;
        vector<string> arquivos = listarArquivosXML("../data"); // Lista os arquivos

        if (index >= 0 && index < arquivos.size())
        {
            string selectedFile = arquivos[index];
            // caminho completo para o arquivo XML
            string fullPath = "../data/" + selectedFile;
            cout << "Processando arquivo XML: " << fullPath << endl;
            processXML(fullPath.c_str()); // Processa o arquivo selecionado
        }
        break;
    }
    }
    glutPostRedisplay(); // Re-desenha a tela
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
    glOrtho(0, view_w, 0, view_h, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glColor3f(1.0f, 0.0f, 0.0f);

    // Restaura as configurações
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}
//____________________________________FIM do MENU___________________________________

//_____________________________________________________________________

// Função para listar arquivos XML no diretório especificado para ao menu
vector<string> Application::listarArquivosXML(const string &directory)
{
    vector<string> arquivos;
    string command = "ls " + directory + "/*.xml > temp.txt"; // Comando para listar arquivos XML
    system(command.c_str());

    ifstream infile("temp.txt");
    string fileName;
    while (getline(infile, fileName))
    {
        cout << "Arquivo encontrado: " << fileName << endl;
        arquivos.push_back(fileName.substr(fileName.find_last_of('/') + 1)); // Extrai apenas o nome do arquivo
    }
    remove("temp.txt");

    if (arquivos.empty())
    {
        cout << "Nenhum arquivo XML encontrado." << endl;
    }

    return arquivos;
}
//---------------------------------------------------------------------
// imprimi a matriz das cores no console para conferir se está tudo correto
void Application::printColorMatrix() const
{
    cout << "Color Matrix:" << endl;
    for (size_t i = 0; i < colorMatrix.size(); ++i)
    {
        cout << "Row " << i << ": ";
        for (size_t j = 0; j < colorMatrix[i].size(); ++j)
        {
            cout << colorMatrix[i][j] << " "; // Imprime a letra
        }
        cout << endl;
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
        int colCount = 0; // Verificar quantas colunas foram lidas
        vector<string> colorRow;
        for (XMLElement *col = row->FirstChildElement("col"); col != nullptr; col = col->NextSiblingElement("col"))
        {
            const char *text = col->GetText();
            if (text)
            {
                string code(text);
                colorRow.push_back(code); // Adiciona o código direto
                colCount++;
            }
        }
        // Verifica se a quantidade de colunas por linha é a esperada
        if (colCount != 4)
        {
            cerr << "Erro: Esperado 4 colunas, mas encontrado " << colCount << endl;
            return; // Sai da função se o formato estiver incorreto
        }
        colorMatrix.push_back(colorRow);
    }
    if (colorMatrix.size() != 4)
    {
        cerr << "Erro: Esperado 4 linhas, mas encontrado " << colorMatrix.size() << endl;
        return; // Sai da função se o número de linhas estiver incorreto
    }

    updateCubeColors();
    printColorMatrix();
}

//---------------------------------------------------------------------
void Application::draw()
{
    glEnable(GL_DEPTH_TEST);// arruma a profundidade
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
    drawFixedText();

    // mostrando
    glutSwapBuffers();
}
//---------------------------------------------------------------------
// Função para desenhar o texto fixo na tela (Não vai girar junto com o Elo)
void Application::drawFixedText()
{

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

    if (gameSolved)
    {
        drawText(-20.0f, -10.f, "O jogo esta resolvido!"); // Adicione a mensagem na tela
    }

    if (gameSave)
    {
        drawText(-20.0f, -16.f, "O jogo esta salvo!"); // Adicione a mensagem na tela
    }

    glColor3f(1.0f, 1.0f, 1.0f); // Cor branca para o texto diferente
    drawText(-60.0f, 100.0f, "Para o MENU use o botao direito do MOUSE");

    drawText(-59.5f, -25.0f, "SETAS para gira o Elo");
    drawText(-59.0f, -30.0f, "F1 e F2 para selecionar os cubos");
    drawText(-58.5f, -35.0f, "CTRL + SETAS para giras os cubos");

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
        else
        {
            textPosY = 7.5f;
        }

        // Desenha a seta na posição do cubo selecionado
        drawText(textPosX, textPosY, "->");
    }

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
            // Ctrl + Seta Esquerda
            rotateLeft();
        }
        else
        {
            // Seta Esquerda é pressionada

            globalRotation -= 5.0f; // Rotaciona 5 graus no sentido anti-horário
        }
        break;

    case GLUT_KEY_RIGHT:
        if (ctrlPressed)
        {
            //  Ctrl + Seta Direita 
            rotateRight();
        }
        else
        {
            // apenas Seta Direita 
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
    }

    glutPostRedisplay(); 
}

//---------------------------------------------------------------------
void Application::update(int value, void (*func_ptr)(int))
{
    glutPostRedisplay();
    glutTimerFunc(30, func_ptr, time);
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
//---------------------------------------------------------------------
// Definição da iluminação da sena
void Application::setLight()
{
    glEnable(GL_LIGHTING);

    GLfloat light_position[] = {50.0f, 50.0f, 50.0f, 1.0f}; // Luz em coordenadas homogêneas

    GLfloat light_ambient[] = {0.3f, 0.3f, 0.3f, 1.0f};  // Luz ambiente
    GLfloat light_diffuse[] = {0.9f, 0.9f, 0.9f, 1.0f};  // Luz difusa
    GLfloat light_specular[] = {0.5f, 0.5f, 0.5f, 1.0f}; // Luz especular

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);

    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    GLfloat lmodel_ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
}
//---------------------------------------------------------------------
// Verificação se o jogo já foi solucionado
bool Application::isSolved()
{
    // Verifica se cada coluna tem apenas uma cor
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
//---------------------------------------------------------------------
// salvando o estado do XML
void Application::saveGameStateToXML()
{
    // Cria o diretório se não existir
    system("mkdir -p data");

    // data e hora atuais e torna em string
    time_t now = std::time(nullptr);
    tm *now_tm = localtime(&now);

    ostringstream oss;
    oss << "../data/estado_jogo_";
    oss << (now_tm->tm_year + 1900) << "-"                     
        << setw(2) << setfill('0') << (now_tm->tm_mon + 1) << "-" 
        << setw(2) << setfill('0') << now_tm->tm_mday << "_"     
        << setw(2) << setfill('0') << now_tm->tm_hour << "-"     
        << setw(2) << setfill('0') << now_tm->tm_min << "-"     
        << setw(2) << setfill('0') << now_tm->tm_sec;            

    // Nome do arquivo com base na data e hora
    string filepath = oss.str() + ".xml";

    // Salva o arquivo XML
    ofstream outFile(filepath);
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
//---------------------------------------------------------------------
// cria um jogo aleatorio
void Application::RandomColorMatrix()
{
    // Lista dos cores a serem adicionada
    vector<string> items = {"vms", "vmm", "vmm", "vmi",
                            "vrs", "vrm", "vrm", "vri",
                            "ams", "amm", "amm", "ami",
                            "brs", "brm", "bri", "vzo"};

    // Embaralha a lista
    random_device rd;
    mt19937 g(rd()); 
    shuffle(items.begin(), items.end(), g);

    colorMatrix.clear(); // Limpa a matriz

    // Preencher a matriz de cores com 4 linhas e 4 colunas
    for (int i = 0; i < 4; ++i)
    {
        vector<string> row;

        for (int j = 0; j < 4; ++j)
        {
            if (!items.empty())
            {
                row.push_back(items.back()); // Pega o último item (aleatório)
                items.pop_back();            // Remove o item da lista
            }
        }
        colorMatrix.push_back(row); // Adiciona a linha à matriz
    }

    printColorMatrix();
    updateCubeColors();
}