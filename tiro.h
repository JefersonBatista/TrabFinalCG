#ifndef TIRO_H
#define TIRO_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>

#include "tempo.h"
#include "util.h"

class Tiro {
    GLfloat inicX, inicY, inicH, cx, cy, h, vel, dir, dirV;
    int dono;
    unsigned long long inic;

    public:
        Tiro(GLfloat cx, GLfloat cy, GLfloat h, GLfloat vel, GLfloat dir, GLfloat dirV, int dono);
        void desenha3d();
        GLfloat nextX();
        GLfloat nextY();
        GLfloat nextH();
        void setX(GLfloat x);
        void setY(GLfloat y);
        void setH(GLfloat h);
        int getDono();
        unsigned long long tempo();
};

#endif
