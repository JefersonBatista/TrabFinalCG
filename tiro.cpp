#include "tiro.h"

Tiro::Tiro(GLfloat cx, GLfloat cy, GLfloat vel, GLfloat dir, int dono) {
    this->inicX = cx;
    this->cx = cx;
    this->inicY = cy;
    this->cy = cy;
    this->vel = vel;
    this->dir = dir;
    this->dono = dono;
    this->inic = iniciar();
}

void Tiro::desenha() {
    glPushMatrix();
        glTranslatef(this->cx, this->cy, 0.0);
        switch (this->dono) {
            case JOGADOR:
                desenhaCirc(RAIO_TIRO, 0.5, 0.5, 0.5);
                break;
            case INIMIGO:
                desenhaCirc(RAIO_TIRO, 0.25, 0.25, 0.0);
        }
    glPopMatrix();
}

GLfloat Tiro::nextX() {
    return this->inicX - vel*this->tempo()*sin(this->dir);
}

GLfloat Tiro::nextY() {
    return this->inicY + vel*this->tempo()*cos(this->dir);
}

void Tiro::setX(GLfloat x) {
    this->cx = x;
}

void Tiro::setY(GLfloat y) {
    this->cy = y;
}

int Tiro::getDono() {
    return dono;
}

unsigned long long Tiro::tempo() {
    return tempo_em_ms(this->inic);
}
