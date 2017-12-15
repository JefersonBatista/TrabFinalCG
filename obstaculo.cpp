#include "obstaculo.h"

Obstaculo::Obstaculo(GLfloat centroX, GLfloat centroY, GLfloat raio) {
    this->cx = centroX;
    this->cy = centroY;
    this->r = raio;
}

GLfloat Obstaculo::getX() {
    return this->cx;
}

GLfloat Obstaculo::getY() {
    return this->cy;
}

GLfloat Obstaculo::getR() {
    return this->r;
}

void Obstaculo::desenha3d(GLfloat altura) {
    glPushMatrix();
        glTranslatef(this->cx, this->cy, 0.0);
        desenhaCilindro(this->r, altura, 0.0, 0.0, 0.0);
    glPopMatrix();
}
