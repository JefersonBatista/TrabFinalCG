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
    GLfloat cx, cy, r, h, front, gun, vel, velTiro, freqTiro;
    GLfloat legRM, legHM, bodyRM, bodyHM, armRM, armHM, headM;
    unsigned long long inicTransicao;
    GLfloat alturaInicTransicao;
    GLfloat legAngle;
    int status;
    unsigned long long lastFootChange;
    unsigned long long lastShoot;
    int movStatus;

    public:
        Inimigo(GLfloat centroX, GLfloat centroY, GLfloat raio, GLfloat vel, GLfloat velTiro, GLfloat freqTiro);
        GLfloat getX();
        GLfloat getY();
        GLfloat getH();
        GLfloat getR();
        GLfloat getAltura();
        GLfloat getAlturaPulo();
        void desenha2d();
        void desenha3d();
        int getMovStatus();
        void setMovStatus(int newMovStatus);
        void move(Jogo *jogo, GLfloat delta, int num);
        void rotate(GLfloat delta);
        void changeLegAngle();
        void moveArma(GLfloat angle);
        void atira(Jogo *jogo);
        void pula(Jogo *jogo);
        void anima(Jogo *jogo);
};

#endif
