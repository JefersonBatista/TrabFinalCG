#ifndef UTIL_H
#define UTIL_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#include <math.h>

// Classe utilitária para definições do jogo e funções de desenho

// Status do jogo
#define JOGANDO 0
#define VENCEU 1
#define PERDEU 2

// Para controlar o passo
#define RIGHTFOOT 0
#define LEFTFOOT 1

// Status dos bonecos
#define NORMAL 0
#define PULANDO 1
#define CAINDO 2
#define PLATAFORMA 3

// Checagem de limites
#define CHAO 0
#define OBSTACULO 1
#define PROIBIDO 2

// Tipos de personagem
#define JOGADOR 0
#define INIMIGO 1

// Duração do pulo
#define DURACAO_PULO 2000

#define RAIO_TIRO 3.0

// Tempo do passo
#define TEMPO_PASSO 200

// Proporção entre o movimento e a rotação dos personagens
#define MOTION_TO_ROTATION 30.0

// Para controle do movimento dos inimigos
#define FORWARD 0
#define CURVE_IN 1
#define FLEE_IN 2
#define CURVE_OUT 3
#define FLEE_OUT 4

void desenhaCirc(GLfloat r, GLfloat R, GLfloat G, GLfloat B);
void desenhaElipse(GLfloat rx, GLfloat ry, GLfloat R, GLfloat G, GLfloat B);
void desenhaRetangulo(GLfloat lx, GLfloat ly, GLfloat R, GLfloat G, GLfloat B);
GLfloat distancia(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
GLfloat nivel(GLfloat alturaInic, GLfloat tempo, int status);
GLfloat zoom(GLfloat altura);

#endif
