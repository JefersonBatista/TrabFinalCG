#ifndef OBSTACULO_H
#define OBSTACULO_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>

#include "util.h"

class Obstaculo {
    GLfloat cx, cy, r;

    public:
        Obstaculo(GLfloat centroX, GLfloat centroY, GLfloat raio);
        GLfloat getX();
        GLfloat getY();
        GLfloat getR();
        void desenha2d();
        void desenha3d(GLfloat altura);
};

#endif
