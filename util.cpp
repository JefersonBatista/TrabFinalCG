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

GLfloat nivel(GLfloat alturaInic, GLfloat tempo, int status) {
    GLfloat result;
    switch (status) {
        case PULANDO:
            result = alturaInic + tempo;
            break;
        case CAINDO:
            result = alturaInic - tempo;
    }
    // printf("nivel = %f\n", result);
    return result;
}

GLfloat zoom(GLfloat altura) {
    GLfloat result = 1.0 + 0.5*altura;
    // printf("zoom = %f\n", result);
    return result;
}
