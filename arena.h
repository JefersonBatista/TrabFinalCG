#ifndef ARENA_H
#define ARENA_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>

#include "util.h"

class Arena {
    GLfloat cxBlue, cyBlue, rBlue, cxWhite, cyWhite, rWhite;

    public:
        Arena(GLfloat centroXBlue, GLfloat centroYBlue, GLfloat raioBlue, GLfloat centroXWhite, GLfloat centroYWhite, GLfloat raioWhite);
        GLfloat getXBlue();
        GLfloat getYBlue();
        GLfloat getRBlue();
        GLfloat getXWhite();
        GLfloat getYWhite();
        GLfloat getRWhite();
        void desenha();
};

#endif
