#include "util.h"

void desenhaCirc(GLfloat r, GLfloat R, GLfloat G, GLfloat B) {
    int points = 200;
    GLfloat angle = 0.0, angleIncr = 2*M_PI/points;

    glColor3f(R,G,B);
    glBegin(GL_POLYGON);
        for(int i = 0; i < points; i++) {
            glVertex3f(r*cos(angle), r*sin(angle), 0.0);
            angle += angleIncr;
        }
    glEnd();
}

void desenhaCircLin(GLfloat r, GLfloat R, GLfloat G, GLfloat B) {
    int points = 200;
    GLfloat angleIncr = 2*M_PI/points;
    GLfloat angle = angleIncr;

    glColor3f(R,G,B);
    glBegin(GL_LINES);
        glVertex3f(r, 0.0, 0.0);
        for(int i = 1; i < points; i++) {
            glVertex3f(r*cos(angle), r*sin(angle), 0.0);
            glVertex3f(r*cos(angle), r*sin(angle), 0.0);
            angle += angleIncr;
        }
        glVertex3f(r, 0.0, 0.0);
    glEnd();
}

void desenhaEsfera(GLfloat r, GLfloat R, GLfloat G, GLfloat B) {
    glutSolidSphere(r, 100, 16);
}

void desenhaCilindro(GLfloat r, GLfloat h, GLfloat R, GLfloat G, GLfloat B) {
    int slices = 100;
    GLfloat x              = 0.0;
    GLfloat y              = 0.0;
    GLfloat angle          = 0.0;
    GLfloat angle_stepsize = 2.0*M_PI/slices;

    /** Draw the tube */
    glColor3f(R,G,B);
    glBegin(GL_QUAD_STRIP);
    angle = 0.0;
        while(angle < 2.0*M_PI) {
            x = r * cos(angle);
            y = r * sin(angle);
            glVertex3f(x, y, h);
            glVertex3f(x, y, 0.0);
            angle += angle_stepsize;
        }
        glVertex3f(r, 0.0, h);
        glVertex3f(r, 0.0, 0.0);
    glEnd();

    /** Draw the circle on top of cylinder */
    glColor3f(R,G,B);
    glBegin(GL_POLYGON);
    angle = 0.0;
        while(angle < 2*M_PI) {
            x = r * cos(angle);
            y = r * sin(angle);
            glVertex3f(x, y, h);
            angle += angle_stepsize;
        }
        glVertex3f(r, 0.0, h);
    glEnd();
}

void desenhaElipse(GLfloat rx, GLfloat ry, GLfloat R, GLfloat G, GLfloat B) {
    int points = 200;
    GLfloat angle = 0.0, angleIncr = 2*M_PI/points;

    glColor3f(R,G,B);
    glBegin(GL_POLYGON);
        for(int i = 0; i < points; i++) {
            glVertex3f(rx*cos(angle), ry*sin(angle), 0.0);
            angle += angleIncr;
        }
    glEnd();
}

void desenhaRetangulo(GLfloat lx, GLfloat ly, GLfloat R, GLfloat G, GLfloat B) {
    glColor3f(R,G,B);
    glBegin(GL_POLYGON);
            glVertex3f(0.0, 0.0, 0.0);
            glVertex3f(lx, 0.0, 0.0);
            glVertex3f(lx, ly, 0.0);
            glVertex3f(0.0, ly, 0.0);
    glEnd();
}

GLfloat distancia(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2) {
    GLfloat distQuadX = (x2 - x1)*(x2 - x1);
    GLfloat distQuadY = (y2 - y1)*(y2 - y1);
    return sqrt(distQuadX + distQuadY);
}

GLfloat nivel(GLfloat alturaInic, GLfloat alturaPulo, GLfloat tempo, int status) {
    /* Dividido por 2 (tempo de subida)
     * Dividido por 1000 (tempoPulo em s, não ms)
     */
    GLfloat tempoPulo = DURACAO_PULO/2000.0;
    switch(status) {
        case PULANDO:
            return alturaInic + alturaPulo*(tempo/tempoPulo);
        case CAINDO:
            return alturaInic - alturaPulo*(tempo/tempoPulo);
    }
}
