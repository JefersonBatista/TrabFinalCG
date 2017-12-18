#include "inimigo.h"

Inimigo::Inimigo(GLfloat centroX, GLfloat centroY, GLfloat raio, GLfloat vel, GLfloat velTiro, GLfloat freqTiro) {
    this->cx = centroX;
    this->cy = centroY;
    this->r = raio;
    this->h = 0.0;
    this->front = 0.0;
    this->gun = 0.0;
    this->vel = vel;
    this->velTiro = velTiro;
    this->freqTiro = freqTiro;
    this->legAngle = 0.0;
    this->status = NORMAL;
    this->lastFootChange = iniciar();
    this->lastShoot = iniciar();
    this->movStatus = FORWARD;

    // Dimensões do corpo do inimigo
    this->legRM = 0.5;
    this->legHM = 1.5;
    this->bodyRM = 1.0;
    this->bodyHM = 1.0;
    this->armRM = 0.3;
    this->armHM = 1.5;
    this->headM = 1.0;
}

GLfloat Inimigo::getX() {
    return this->cx;
}

GLfloat Inimigo::getY() {
    return this->cy;
}

GLfloat Inimigo::getH() {
    return this->h;
}

GLfloat Inimigo::getR() {
    return this->r;
}

GLfloat Inimigo::getAltura() {
    return r*legHM + r*bodyHM + 2*r*headM;
}

GLfloat Inimigo::getAlturaPulo() {
    return getAltura()/3.0;
}

void Inimigo::desenha2d() {
    glPushMatrix();
        glTranslatef(this->cx, this->cy, 0.0);
        desenhaCirc(this->r, 1.0, 0.0, 0.0);
    glPopMatrix();
}

void Inimigo::desenha3d() {
    // printf("cx = %f, cy = %f, r = %f, h = %f\n", cx, cy, r, h);
    GLfloat frontDeg = front/(2*M_PI)*360.0;

    glPushMatrix();
        glTranslatef(cx, cy, h);
        glRotatef(frontDeg, 0.0, 0.0, 1.0);

        // Desenhando as pernas
        glPushMatrix();
            glTranslatef(r*legRM, 0.0, 0.0);
            desenhaCilindro(r*legRM, r*legHM, 0.15, 0.0, 0.0);
            glTranslatef(-2.0*(r*legRM), 0.0, 0.0);
            desenhaCilindro(r*legRM, r*legHM, 0.15, 0.0, 0.0);
        glPopMatrix();

        // Desenhando o tronco
        glTranslatef(0.0, 0.0, r*legHM);
        desenhaCilindro(r*bodyRM, r*bodyHM, 0.3, 0.0, 0.3);

        // Desenhando o braço
        glTranslatef(0.0, 0.0, r*bodyHM);
        glPushMatrix();
            GLfloat gunDeg = this->gun/(2*M_PI)*360.0;
            glTranslatef(r*bodyRM, 0.0, 0.0);
            glRotatef(gunDeg, 0.0, 0.0, 1.0);
            glRotatef(-90.0, 1.0, 0.0, 0.0);
            desenhaCilindro(r*armRM, r*armHM, 0.5, 0.0, 0.0);
        glPopMatrix();

        // Desenhando ombros
        // desenhaElipse(this->r, this->r/3.0, 0.0, 0.6, 0.0);

        // Desenhando a cabeça
        glTranslatef(0.0, 0.0, r*headM);
        desenhaEsfera(r*headM, 1.0, 0.0, 0.0);
    glPopMatrix();
}

int Inimigo::getMovStatus() {
    return this->movStatus;
}

void Inimigo::setMovStatus(int newMovStatus) {
    this->movStatus = newMovStatus;
}

void Inimigo::move(Jogo *jogo, GLfloat delta, int num) {
    GLfloat incr = this->vel*delta;

    GLfloat pX = this->cx - incr*sin(this->front);
    GLfloat pY = this->cy + incr*cos(this->front);
    GLfloat r = this->r;

    int checagem = jogo->checarInimigos(pX, pY, r, num);

    // Decide se move o inimigo
    switch(checagem) {
        case CHAO:
            this->cx = pX;
            this->cy = pY;
            this->changeLegAngle();
            if(this->status == PLATAFORMA) {
                this->alturaInicTransicao = jogo->getAlturaObst();
                this->inicTransicao = iniciar();
                this->status = CAINDO;
            }
            return;
        case OBSTACULO:
            switch(this->status) {
                case NORMAL:
                    return;
                case PULANDO:
                    this->h = nivel(this->alturaInicTransicao, this->getAlturaPulo(), tempo_em_s(this->inicTransicao), this->status);
                    if (this->h >= jogo->getAlturaObst()) {
                        this->cx = pX;
                        this->cy = pY;
                        this->changeLegAngle();
                    } else {
                        return;
                    }
                case CAINDO:
                    this->h = nivel(this->alturaInicTransicao, this->getAlturaPulo(),tempo_em_s(this->inicTransicao), this->status);
                    if (this->h >= jogo->getAlturaObst()) {
                        this->cx = pX;
                        this->cy = pY;
                        this->changeLegAngle();
                    } else {
                        return;
                    }
                case PLATAFORMA:
                    this->cx = pX;
                    this->cy = pY;
                    this->changeLegAngle();
            }
        default:
            // Caso PROIBIDO, não faça o movimento
            return;
    }
}

void Inimigo::rotate(GLfloat delta) {
    // printf("Uai, sô!\n");
    GLfloat angleIncr = this->vel*delta/MOTION_TO_ROTATION;

    this->front += angleIncr;
    if(this->front > 2*M_PI) this->front -= 2*M_PI;
}

// TODO: Como controlar os ângulos das pernas?
void Inimigo::changeLegAngle() {
    unsigned long long tempo = tempo_em_ms(this->lastFootChange);
    this->legAngle += tempo/100.0;
    if(this->legAngle > M_PI/4.0) {
        this->legAngle -= M_PI/2.0;
    }
    this->lastFootChange = iniciar();
}

void Inimigo::moveArma(GLfloat angle) {
    GLfloat limit = (45.0/360.0)*2*M_PI;

    if(this->gun + angle > limit || this->gun + angle < -limit) {
        // Limite do movimento do braço alcançado, não mover
        return;
    } else {
        this->gun += angle;
    }
}

void Inimigo::atira(Jogo *jogo) {
    GLfloat toArm = r*bodyRM;
    GLfloat angle = this->front;

    GLfloat cx = this->cx + toArm*cos(angle);
    GLfloat cy = this->cy + toArm*sin(angle);
    GLfloat h = this->h + r*legHM + r*bodyHM;
    GLfloat vel = this->velTiro;
    GLfloat dir = this->front + this->gun;

    jogo->adicionarTiro(Tiro(cx, cy, h, vel, dir, 0.0, INIMIGO));
}

void Inimigo::pula(Jogo *jogo) {
    switch (this->status) {
        case NORMAL:
            this->alturaInicTransicao = 0.0;
            this->inicTransicao = iniciar();
            this->status = PULANDO;
            break;
        case PLATAFORMA:
            this->alturaInicTransicao = jogo->getAlturaObst();
            this->inicTransicao = iniciar();
            this->status = PULANDO;
    }
}

void Inimigo::anima(Jogo *jogo) {
    int checagem;
    GLfloat tempo;
    GLfloat tempoPulo;

    switch (this->status) {
        case PULANDO:
            /* Dividido por 2 (tempo de subida)
             * Dividido por 1000 (tempoPulo em s, não ms)
             */
            tempoPulo = DURACAO_PULO/2000.0;
            tempo = tempo_em_s(this->inicTransicao);

            // O pulo acabou?
            if(tempo > tempoPulo) {
                this->alturaInicTransicao += (1.0 - (tempo - tempoPulo)/tempoPulo)*this->getAlturaPulo();
                this->inicTransicao = iniciar();
                this->status = CAINDO;
            }
            this->h = nivel(this->alturaInicTransicao, this->getAlturaPulo(),tempo_em_s(this->inicTransicao), this->status);
            break;
        case CAINDO:
            checagem = jogo->checarLimites(this->cx, this->cy, this->r, INIMIGO);
            this->h = nivel(this->alturaInicTransicao, this->getAlturaPulo(),tempo_em_s(this->inicTransicao), this->status);

            switch (checagem) {
                case CHAO:
                    // A queda acabou?
                    if(this->h < 0.0) {
                        this->h = 0.0;
                        this->status = NORMAL;
                    }
                    break;
                case OBSTACULO:
                    // A queda acabou?
                    if(this->h < jogo->getAlturaObst()) {
                        this->h = jogo->getAlturaObst();
                        this->status = PLATAFORMA;
                    }
            }
    }

    GLfloat tempoPorTiro = 1.0/this->freqTiro;
    if(tempo_em_s(this->lastShoot) > tempoPorTiro) {
        this->atira(jogo);
        this->lastShoot = iniciar();
    }
}
