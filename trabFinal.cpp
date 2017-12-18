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
#define WindowHeight 700
#define MouseMotionToGunRotation 8.0
#define MouseExitDetection 0.02

#define LEFT_BUTTON 0
#define RIGHT_BUTTON 2

//Camera controls (not used yet, except camAngle)
double camDist=50;
double camHAngle=0;
double camVAngle=0;
int toggleCam = 1;
int camAngle = 90;
int lastX = 0;
int lastY = 0;
int buttonDown=0;

Arena *arena;
int arenaData[6];

GLfloat prevX, prevY;

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

void renderScene(int braco) {

    jogo->desenha(braco);

}

void renderMinimap(void) {
    jogo->minimapa();
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // parte de cima - Visao do Jogador
    glMatrixMode(GL_PROJECTION);
    glViewport(0,WindowWidth, WindowWidth, 200);
    glLoadIdentity();
    gluPerspective(camAngle, WindowWidth/200, (ViewingYMax - ViewingYMin)/1000, (ViewingYMax - ViewingYMin)*50.0);

    GLfloat recuo = jogador->getR();
    GLfloat ang = jogador->getFront();
    gluLookAt(jogador->getX()-recuo*sin(ang), jogador->getY()+recuo*cos(ang), jogador->getH()+jogador->getAltura()*4/5,
        jogador->getX()-2*recuo*sin(ang), jogador->getY()+2*recuo*cos(ang), jogador->getH()+jogador->getAltura()*4/5,
        0, 0, 1);
    glMatrixMode(GL_MODELVIEW);


    renderScene(0); // desenha mundo


    // parte de baixo - Visao das Camera
    glMatrixMode(GL_PROJECTION);
    glViewport(0,0, WindowWidth, WindowWidth);
    glLoadIdentity();
    gluPerspective(camAngle, 1.0, (ViewingYMax - ViewingYMin)/1000, (ViewingYMax - ViewingYMin)*50.0);

    if (toggleCam == 1){
        // camera sobre a arma
        GLfloat h = jogador->getAltura()*3.5/5 + jogador->getH();
        GLfloat recuo = jogador->getR();
        GLfloat front90 = jogador->getFront()+90*M_PI/180;
        GLfloat front45 = front90+45*M_PI/180;
        GLfloat angH = jogador->getGunH() ;
        GLfloat angV = jogador->getGunV();
        if(angH < 0)
            angH = angH/1.5;
        gluLookAt(jogador->getX()+recuo*sin(front90), jogador->getY()-recuo*cos(front90),h,
            jogador->getX()+recuo*sqrt(2)*sin(front45+angH), jogador->getY()-recuo*sqrt(2)*cos(front45+angH), h+recuo*sqrt(2)*tan(angV),
            0, 0, 1);
    }

    if (toggleCam == 2){
        // camera 3a pessoa
        GLfloat recuo = 5*jogador->getR();
        GLfloat angH = jogador->getFront();
        GLfloat angV = camVAngle*M_PI/180;
        angH -= camHAngle*M_PI/180;

        GLfloat Xcam = jogador->getX()+(recuo)*sin(angH)*cos(angV);
        GLfloat Ycam = jogador->getY()-(recuo)*cos(angH)*cos(angV);
        GLfloat Hcam = jogador->getAltura()+jogador->getAltura()*sin(angV);
        gluLookAt(Xcam, Ycam, Hcam,
            jogador->getX(), jogador->getY(), jogador->getAltura()/2, // mira no meio do tronco do jogador
            0, 0, 1);
    }

    glMatrixMode(GL_MODELVIEW);

    if(toggleCam == 2)
        renderScene(0); // desenha mundo
    else
        renderScene(1); // desenha só o braço
    jogo->mensagem();

    // desenha minimapa
    glMatrixMode(GL_PROJECTION);
    glViewport(WindowWidth*3/4,0, WindowWidth/4, WindowWidth/4);
    glLoadIdentity();

    glOrtho(
        ViewingXMin/2,
        ViewingXMax/2,
        ViewingYMin/2,
        ViewingYMax/2,
        100,
        -100
    );

    glMatrixMode(GL_MODELVIEW);

    renderMinimap(); // desenha minimapa do mundo

    glutSwapBuffers();
}

void mouse(int button, int state, int x, int y) {
    if(!state && button == LEFT_BUTTON) {
        jogador->atira(jogo);
    }
    if(!state && button == RIGHT_BUTTON) {
        buttonDown = 1;
    }else{
        buttonDown = 0;
    }
}

void mouseMotion(int x, int y) {
    GLfloat gx = (GLfloat) x/WindowWidth;
    GLfloat gy = (GLfloat) y/WindowWidth;
    GLfloat angle;
    GLfloat angleV;

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

    // Se a posição do mouse ainda não foi inicializada
    if(prevY < 0.0) {
        prevY = gy;
    }
    // Caso em que o mouse retorna à tela de jogo
    else if(gy - prevY > MouseExitDetection || gy - prevY < -MouseExitDetection) {
        prevY = gy;
    }
    // Movendo o braço do jogador
    else {
        /* Rotação por um ângulo positivo, caso o movimento do mouse
         * seja para a esquerda, e negativo, caso seja para a direita.
         */
        angleV = -MouseMotionToGunRotation*(gy - prevY);
        jogador->moveArmaV(angleV);
        prevY = gy;
    }

    if (buttonDown){
        if((camHAngle + x - lastX) > -180 && (camHAngle + x - lastX) < 180 ){
            camHAngle += x - lastX;
        }
        if((camVAngle + y - lastY) > -90 && (camVAngle + y - lastY) < 90 ){
            camVAngle += y - lastY;
        }
    }
    lastX = x;
    lastY = y;

    glutPostRedisplay();
}

void keyPress(unsigned char key, int x, int y) {

    if(key == '1') {
        toggleCam = 1;
    }
    if(key == '2') {
        toggleCam = 2;
    }
    if(key == 'p' || key == ' ') {
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
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
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
    // glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
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
