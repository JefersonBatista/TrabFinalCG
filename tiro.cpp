#include "tiro.h"

Tiro::Tiro(GLfloat cx, GLfloat cy, GLfloat h, GLfloat vel, GLfloat dir, GLfloat dirV, int dono) {
    this->inicX = cx;
    this->cx = cx;
    this->inicY = cy;
    this->cy = cy;
    this->inicH = h;
    this->h = h;
    this->vel = vel;
    this->dir = dir;
    this->dirV = dirV;
    this->dono = dono;
    this->inic = iniciar();
}

void Tiro::desenha3d() {
    glPushMatrix();
        glTranslatef(this->cx, this->cy, this->h);
        switch (this->dono) {
            case JOGADOR:
                desenhaEsfera(RAIO_TIRO, 0.5, 0.5, 0.5);
                break;
            case INIMIGO:
                desenhaEsfera(RAIO_TIRO, 0.25, 0.25, 0.0);
        }
    glPopMatrix();
}

GLfloat Tiro::nextX() {
    return this->inicX - vel*this->tempo()*sin(this->dir);
}

GLfloat Tiro::nextY() {
    return this->inicY + vel*this->tempo()*cos(this->dir);
}

GLfloat Tiro::nextH() {
    return this->inicH + vel*this->tempo()*cos(this->dirV);
}

void Tiro::setX(GLfloat x) {
    this->cx = x;
}

void Tiro::setY(GLfloat y) {
    this->cy = y;
}

void Tiro::setH(GLfloat h) {
    this->h = h;
}

int Tiro::getDono() {
    return dono;
}

unsigned long long Tiro::tempo() {
    return tempo_em_ms(this->inic);
}
