#include "arena.h"

Arena::Arena(GLfloat centroXBlue, GLfloat centroYBlue, GLfloat raioBlue, GLfloat centroXWhite, GLfloat centroYWhite, GLfloat raioWhite) {
    this->cxBlue = centroXBlue;
    this->cyBlue = centroYBlue;
    this->rBlue = raioBlue;

    this->cxWhite = centroXWhite;
    this->cyWhite = centroYWhite;
    this->rWhite = raioWhite;
}

GLfloat Arena::getXBlue() {
    return this->cxBlue;
}

GLfloat Arena::getYBlue() {
    return this->cyBlue;
}

GLfloat Arena::getRBlue() {
    return this->rBlue;
}

GLfloat Arena::getXWhite() {
    return this->cxWhite;
}

GLfloat Arena::getYWhite() {
    return this->cyWhite;
}

GLfloat Arena::getRWhite() {
    return this->rWhite;
}

void Arena::desenha2d() {
    glPushMatrix();
        glTranslatef(this->cxBlue, this->cyBlue, 0.0);
        desenhaCircLin(this->rBlue, 0.5, 0.5, 0.0);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(this->cxWhite, this->cyWhite, 0.0);
        desenhaCircLin(this->rWhite, 0.5, 0.5, 0.0);
    glPopMatrix();
}

void Arena::desenha3d(GLfloat altura) {
    glPushMatrix();
        glTranslatef(this->cxBlue, this->cyBlue, 0.0);
        // desenhaCilindro(this->rBlue, 120, 0.0, 0.0, 1.0);
        desenhaCirc(this->rBlue, 0.0, 0.0, 1.0);
        desenhaCilindro(this->rBlue, altura, 0.25, 0.25, 0.0);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(this->cxWhite, this->cyWhite, 0.0);
        desenhaCilindro(this->rWhite, altura, 0.5, 0.5, 0.5);
    glPopMatrix();
}
