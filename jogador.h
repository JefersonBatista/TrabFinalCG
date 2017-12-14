#ifndef JOGADOR_H
#define JOGADOR_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>

#include "tempo.h"
#include "util.h"
#include "tiro.h"
#include "jogo.h"

class Jogo;

class Jogador {
    GLfloat cx, cy, r, h, front, gun, vel, velTiro;
    unsigned long long inicTransicao;
    GLfloat alturaInicTransicao;
    GLfloat legAngle;
    int status;
    unsigned long long lastFootChange;

    public:
        Jogador(GLfloat centroX, GLfloat centroY, GLfloat raio, GLfloat vel, GLfloat velTiro);
        GLfloat getX();
        GLfloat getY();
        GLfloat getR();
        GLfloat getAltura();
        GLfloat getFront();
        void desenha3d();
        void desenha2d();
        void move(Jogo *jogo, GLfloat delta);
        void rotate(GLfloat delta);
        void changeLegAngle();
        void moveArma(GLfloat angle);
        void atira(Jogo *jogo);
        void pula(Jogo *jogo);
        void anima(Jogo *jogo);
};

#endif
