#ifndef TIRO_H
#define TIRO_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>

#include "tempo.h"
#include "util.h"

class Tiro {
    GLfloat inicX, inicY, cx, cy, vel, dir;
    int dono;
    unsigned long long inic;

    public:
        Tiro(GLfloat cx, GLfloat cy, GLfloat vel, GLfloat dir, int dono);
        void desenha();
        GLfloat nextX();
        GLfloat nextY();
        void setX(GLfloat x);
        void setY(GLfloat y);
        int getDono();
        unsigned long long tempo();
};

#endif
