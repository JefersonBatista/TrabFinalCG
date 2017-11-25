#ifndef INIMIGO_H
#define INIMIGO_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>

#include "tempo.h"
#include "util.h"
#include "tiro.h"
#include "jogo.h"

class Jogo;

class Inimigo {
    GLfloat cx, cy, r, ra, front, gun, vel, velTiro, freqTiro;
    unsigned long long inicTransicao;
    GLfloat alturaInicTransicao;
    int foot;
    int status;
    unsigned long long lastFootChange;
    unsigned long long lastShoot;
    int movStatus;

    public:
        Inimigo(GLfloat centroX, GLfloat centroY, GLfloat raio, GLfloat vel, GLfloat velTiro, GLfloat freqTiro);
        GLfloat getX();
        GLfloat getY();
        GLfloat getR();
        int getMovStatus();
        void setMovStatus(int newMovStatus);
        void desenha();
        void move(Jogo *jogo, GLfloat delta, int num);
        void rotate(GLfloat delta);
        void toogleFoot();
        void moveArma(GLfloat angle);
        void atira(Jogo *jogo);
        void pula(Jogo *jogo);
        void anima(Jogo *jogo);
};

#endif
