#include <GL/glut.h>
#include <math.h>

#include "tinyxml2.h"
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <ctime>

#include "jogo.h"

using namespace tinyxml2;
using namespace std;

#define WindowWidth 500
#define WindowHeight 500
#define MouseMotionToGunRotation 8.0
#define MouseExitDetection 0.02

#define LEFT_BUTTON 0

//Camera controls (not used yet, except camAngle)
double camDist=50;
double camXYAngle=0;
double camXZAngle=0;
int toggleCam = 0;
int camAngle = 90;
int lastX = 0;
int lastY = 0;
int buttonDown=0;

GLfloat prevX;

unsigned long long lastFrame;
GLfloat transcorrido;
int keyFlags[256];

int ViewingXMin, ViewingXMax;
int ViewingYMin, ViewingYMax;
int camPosX, camPosY;

Jogo *jogo;
Jogador *jogador;
GLfloat velTiro;
GLfloat vel;

vector<Inimigo> *inimigos;
GLfloat velTiroInimigo;
GLfloat velInimigo;
GLfloat freqTiroInimigo;

GLfloat alturaObst;

void renderScene(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    jogo->desenha();

    glutSwapBuffers();
}

void display(void) {
    renderScene();
}

void mouse(int button, int state, int x, int y) {
    if(!state && button == LEFT_BUTTON) {
        jogador->atira(jogo);
    }
}

void mouseMotion(int x, int y) {
    GLfloat gx = (GLfloat) x/WindowWidth;
    GLfloat angle;

    // Se a posição do mouse ainda não foi inicializada
    if(prevX < 0.0) {
        prevX = gx;
    }

    // Caso em que o mouse retorna à tela de jogo
    else if(gx - prevX > MouseExitDetection || gx - prevX < -MouseExitDetection) {
        prevX = gx;
    }

    // Movendo o braço do jogador
    else {
        /* Rotação por um ângulo positivo, caso o movimento do mouse
         * seja para a esquerda, e negativo, caso seja para a direita.
         */
        angle = -MouseMotionToGunRotation*(gx - prevX);
        jogador->moveArma(angle);
        prevX = gx;
    }

    glutPostRedisplay();
}

void keyPress(unsigned char key, int x, int y) {
    if(key == 'p') {
        jogador->pula(jogo);
    }
    keyFlags[key] = 1;

    glutPostRedisplay();
}

void keyFree(unsigned char key, int x, int y) {
    keyFlags[key] = 0;

    glutPostRedisplay();
}

void idle(void) {
    // Usando o tempo transcorrido desde o último frame
    transcorrido = tempo_em_ms(lastFrame);
    lastFrame = iniciar();

    if(keyFlags['w']) {
        jogador->move(jogo, +transcorrido);
    }
    if(keyFlags['s']) {
        jogador->move(jogo, -transcorrido);
    }
    if(keyFlags['a']) {
        if(keyFlags['s'] && !keyFlags['w']) {
            jogador->rotate(-transcorrido);
        } else {
            jogador->rotate(+transcorrido);
        }
    }
    if(keyFlags['d']) {
        if(keyFlags['s'] && !keyFlags['w']) {
            jogador->rotate(+transcorrido);
        } else {
            jogador->rotate(-transcorrido);
        }
    }

    jogo->moveInimigos(transcorrido);
    jogo->anima();

    glutPostRedisplay();
}

void init(void) {
    /* selecionar cor de fundo (branco) */
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    /* inicializar sistema de viz. */
    glMatrixMode(GL_PROJECTION);
    /* glOrtho(ViewingXMin,
            ViewingXMax,
            ViewingYMin,
            ViewingYMax,
            100,
            -100); */
    glLoadIdentity();
    gluPerspective(camAngle, 1.0, (ViewingYMax - ViewingYMin)/8.0, (ViewingYMax - ViewingYMin));
    gluLookAt(camPosX, camPosY, (ViewingYMax - ViewingYMin)/4.0,
              camPosX, camPosY, 0,
               0, 1, 0);
    glMatrixMode(GL_MODELVIEW);
    // glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

int main(int argc, char** argv) {
    if(argc < 2) {
        printf("Usage: ./trabalhocg <path of config file>\n");
        return 1;
    }

    // Printando o endereço do arquivo de configuração
    char *configFolder = argv[1];
    const char *configName = "config.xml";
    char configFile[1000];
    sprintf(configFile, "%s%s", configFolder, configName);
    printf("configFilePath: %s\n", configFile);

    // Lendo o arquivo XML de configuração
    XMLDocument doc;
    doc.LoadFile(configFile);
    XMLElement *aplicacao = doc.FirstChildElement();
    if(aplicacao == NULL) {
        printf("aplicacao: NULL\n");
        printf("Arquivo de configuração não pôde ser lido ou não foi encontrado!\n");
        return 2;
    }
    XMLElement *arquivoDaArena = aplicacao->FirstChildElement();

    const char *nome, *tipo, *caminho;
    nome = arquivoDaArena->Attribute("nome");
    tipo = arquivoDaArena->Attribute("tipo");
    caminho = arquivoDaArena->Attribute("caminho");

    XMLElement *jogadorLido = aplicacao->FirstChildElement("jogador");
    jogadorLido->QueryFloatAttribute("velTiro", &velTiro);
    jogadorLido->QueryFloatAttribute("vel", &vel);

    XMLElement *inimigoLido = aplicacao->FirstChildElement("inimigo");
    inimigoLido->QueryFloatAttribute("freqTiro", &freqTiroInimigo);
    inimigoLido->QueryFloatAttribute("velTiro", &velTiroInimigo);
    inimigoLido->QueryFloatAttribute("vel", &velInimigo);

    XMLElement *obstaculoLido = aplicacao->FirstChildElement("obstaculo");
    obstaculoLido->QueryFloatAttribute("altura", &alturaObst);
    alturaObst /= 100.0;
    // Fim da leitura do arquivo de configuração

    // Printando, quanto ao jogador, velocidade e velocidade do tiro
    printf("Velocidade do tiro do jogador: %f\n", velTiro);
    printf("Velocidade do jogador: %f\n\n", vel);

    // Printando, quanto ao inimigo, frequência do tiro, velocidade e velocidade do tiro
    printf("Frequência do tiro do inimigo: %f\n", freqTiroInimigo);
    printf("Velocidade do tiro do inimigo: %f\n", velTiroInimigo);
    printf("Velocidade do inimigo: %f\n\n", velInimigo);

    // Printando a altura dos obstáculos
    printf("Altura dos obstáculos: %f\n\n", alturaObst);

    // Printando o endereço do arquivo da arena
    char arenaFile[1000];
    sprintf(arenaFile, "%s%s.%s", caminho, nome, tipo);
    printf("arenaFilePath: %s\n", arenaFile);

    // Lendo o arquivo XML da arena
    doc.LoadFile(arenaFile);
    XMLElement *svg =doc.FirstChildElement();
    if(svg == NULL) {
        printf("svg: NULL\n");
        printf("Arquivo da arena não pôde ser lido ou não foi encontrado!\n");
        return 3;
    }

    Arena *arena;
    int arenaData[6];

    inimigos = new vector<Inimigo>;
    vector<Obstaculo> *obstaculos = new vector<Obstaculo>;

    XMLElement *circle = svg->FirstChildElement();
    GLfloat cx, cy, r;
    const char *fill;
    while(circle != NULL) {
        circle->QueryFloatAttribute("cx", &cx);
        circle->QueryFloatAttribute("cy", &cy);
        cy = -cy;
        circle->QueryFloatAttribute("r", &r);
        fill = circle->Attribute("fill");

        if(0 == strcmp(fill, "blue")) {
                arenaData[0] = cx;
                arenaData[1] = cy;
                arenaData[2] = r;
        } else if(0 == strcmp(fill, "white")) {
                arenaData[3] = cx;
                arenaData[4] = cy;
                arenaData[5] = r;
        } else if(0 == strcmp(fill, "green")) {
                jogador = new Jogador(cx, cy, r, vel, velTiro);
        } else if(0 == strcmp(fill, "black")) {
                obstaculos->push_back(Obstaculo(cx, cy, r));
        } else if(0 == strcmp(fill, "red")) {
                inimigos->push_back(Inimigo(cx, cy, r, velInimigo, velTiroInimigo, freqTiroInimigo));
        } else {
                printf("Círculo ignorado por não pertencer ao escopo do jogo!\n");
        }

        circle = circle->NextSiblingElement();
    }
    // Fim da leitura do arquivo da arena

    arena = new Arena(arenaData[0], arenaData[1], arenaData[2],
                      arenaData[3], arenaData[4], arenaData[5]);

    jogo = new Jogo(arena, jogador, inimigos, obstaculos, alturaObst);

    // Para a arena se ajustar à tela
    ViewingXMin = 2*(arenaData[0] - arenaData[2]);
    ViewingXMax = 2*(arenaData[0] + arenaData[2]);
    ViewingYMin = 2*(arenaData[1] - arenaData[2]);
    ViewingYMax = 2*(arenaData[1] + arenaData[2]);
    camPosX = arenaData[0];
    camPosY = arenaData[1];

    for(int i = 0; i < 256; i++) {
        keyFlags[i] = 0;
    }

    prevX = -1.0;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WindowWidth, WindowHeight);
    glutInitWindowPosition(400, 100);
    glutCreateWindow("Arena");
    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    glutPassiveMotionFunc(mouseMotion);
    glutKeyboardFunc(keyPress);
    glutKeyboardUpFunc(keyFree);
    glutIdleFunc(idle);
    glutMainLoop();

    /* C ANSI requer que main retorne um inteiro */
    return 0;
}
