#include "jogador.h"

Jogador::Jogador(GLfloat centroX, GLfloat centroY, GLfloat raio, GLfloat vel, GLfloat velTiro) {
    this->cx = centroX;
    this->cy = centroY;
    this->r = raio;
    this->h = 0.0;
    this->front = 0.0;
    this->gun = 0.0;
    this->vel = vel;
    this->velTiro = velTiro;
    this->legAngle = 0.0;
    this->status = NORMAL;
    this->lastFootChange = iniciar();

    // Dimensões do corpo do jogador
    this->legRM = 0.5;
    this->legHM = 1.5;
    this->bodyRM = 1.0;
    this->bodyHM = 1.0;
    this->armRM = 0.3;
    this->armHM = 1.5;
    this->headM = 1.0;
}

GLfloat Jogador::getX() {
    return this->cx;
}

GLfloat Jogador::getY() {
    return this->cy;
}

GLfloat Jogador::getR() {
    return this->r;
}

void Jogador::desenha2d() {
    GLfloat frontDeg = this->front/(2*M_PI)*360.0;

    glPushMatrix();
        glTranslatef(this->cx, this->cy, 0.0);
        glRotatef(frontDeg, 0.0, 0.0, 1.0);

        // Desenhando os pés
        /* switch(this->foot) {
            case RIGHTFOOT:
                desenhaRetangulo(this->r/2.25, this->r/1.2, 0.15, 0.0, 0.0);
                desenhaRetangulo(-this->r/2.25, -this->r/1.2, 0.15, 0.0, 0.0);
                break;
            case LEFTFOOT:
                desenhaRetangulo(-this->r/2.25, this->r/1.2, 0.15, 0.0, 0.0);
                desenhaRetangulo(this->r/2.25, -this->r/1.2, 0.15, 0.0, 0.0);
        } */

        // Desenhando o braço
        glPushMatrix();
            GLfloat gunDeg = this->gun/(2*M_PI)*360.0;
            glTranslatef(this->r/1.5, 0.0, 0.0);
            glRotatef(gunDeg, 0.0, 0.0, 1.0);
            desenhaRetangulo(this->r/4.0, this->r, 0.5, 0.0, 0.0);
        glPopMatrix();

        // Desenhando ombros
        desenhaElipse(this->r, this->r/3.0, 0.6, 0.0, 0.0);

        // Desenhando a cabeça
        desenhaCirc(this->r/1.5, 1.0, 0.0, 0.0);
    glPopMatrix();
}

GLfloat Jogador::getAltura(){
    return r*legHM + r*bodyHM + 2*r*headM;
}

GLfloat Jogador::getFront() {
    return this->front;
}

void Jogador::desenha3d() {
    GLfloat frontDeg = front/(2*M_PI)*360.0;

    glPushMatrix();
        glTranslatef(cx, cy, h);
        glRotatef(frontDeg, 0.0, 0.0, 1.0);

        // Desenhando as pernas
        glPushMatrix();
            glTranslatef(r*legRM, 0.0, 0.0);
            desenhaCilindro(r*legRM, r*legHM, 0.0, 0.15, 0.0);
            glTranslatef(-2.0*(r*legRM), 0.0, 0.0);
            desenhaCilindro(r*legRM, r*legHM, 0.0, 0.15, 0.0);
        glPopMatrix();

        // Desenhando o tronco
        glTranslatef(0.0, 0.0, r*legHM);
        desenhaCilindro(r*bodyRM, r*bodyHM, 0.0, 0.3, 0.3);

        // Desenhando o braço
        glTranslatef(0.0, 0.0, r*bodyHM);
        glPushMatrix();
            GLfloat gunDeg = this->gun/(2*M_PI)*360.0;
            glTranslatef(r*bodyRM, 0.0, 0.0);
            glRotatef(gunDeg, 0.0, 0.0, 1.0);
            glRotatef(-90.0, 1.0, 0.0, 0.0);
            desenhaCilindro(r*armRM, r*armHM, 0.0, 0.5, 0.0);
        glPopMatrix();

        // Desenhando ombros
        // desenhaElipse(this->r, this->r/3.0, 0.0, 0.6, 0.0);

        // Desenhando a cabeça
        glTranslatef(0.0, 0.0, r*headM);
        desenhaEsfera(r*headM, 0.0, 1.0, 0.0);
    glPopMatrix();
}

void Jogador::move(Jogo *jogo, GLfloat delta) {
    GLfloat incr = this->vel*delta;

    GLfloat pX = this->cx - incr*sin(this->front);
    GLfloat pY = this->cy + incr*cos(this->front);
    GLfloat r = this->r;

    int checagem = jogo->checarLimites(pX, pY, r, JOGADOR);

    // Decide se move o jogador
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
                    this->h = nivel(this->alturaInicTransicao, tempo_em_s(this->inicTransicao), this->status);
                    if (this->h >= jogo->getAlturaObst()) {
                        this->cx = pX;
                        this->cy = pY;
                        this->changeLegAngle();
                    } else {
                        return;
                    }
                case CAINDO:
                    this->h = nivel(this->alturaInicTransicao, tempo_em_s(this->inicTransicao), this->status);
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

void Jogador::rotate(GLfloat delta) {
    GLfloat angleIncr = this->vel*delta/MOTION_TO_ROTATION;

    this->front += angleIncr;
    if(this->front > 2*M_PI) this->front -= 2*M_PI;
}

// TODO: Como controlar os ângulos das pernas?
void Jogador::changeLegAngle() {
    unsigned long long tempo = tempo_em_ms(this->lastFootChange);
    this->legAngle += tempo/100.0;
    if(this->legAngle > M_PI/4.0) {
        this->legAngle -= M_PI/2.0;
    }
    this->lastFootChange = iniciar();
}

void Jogador::moveArma(GLfloat angle) {
    GLfloat limit = (45.0/360.0)*2*M_PI;

    if(this->gun + angle > limit || this->gun + angle < -limit) {
        // Limite do movimento do braço alcançado, não mover
        return;
    } else {
        this->gun += angle;
    }
}

void Jogador::atira(Jogo *jogo) {
    // Não atirar pulando ou em cima de plataforma
    if(this->status != NORMAL) {
        return;
    }

    GLfloat toArm = this->r/1.5 + this->r/8.0;
    GLfloat angle = this->front;

    GLfloat cx = this->cx + toArm*cos(angle);
    GLfloat cy = this->cy + toArm*sin(angle);
    GLfloat h = r*legHM + r*bodyHM;
    GLfloat vel = this->velTiro;
    GLfloat dir = this->front + this->gun;

    jogo->adicionarTiro(Tiro(cx, cy, h, vel, dir, JOGADOR));
}

void Jogador::pula(Jogo *jogo) {
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

void Jogador::anima(Jogo *jogo) {
    int checagem;
    GLfloat mult;
    GLfloat tempo;

    switch (this->status) {
        case PULANDO:
            tempo = tempo_em_s(this->inicTransicao);

            // O pulo acabou?
            if(tempo > 1.0) {
                this->alturaInicTransicao += 1.0 - (tempo - 1.0);
                this->inicTransicao = iniciar();
                this->status = CAINDO;
            }
            this->h = nivel(this->alturaInicTransicao, tempo_em_s(this->inicTransicao), this->status);
            break;
        case CAINDO:
            checagem = jogo->checarLimites(this->cx, this->cy, this->r, JOGADOR);
            this->h = nivel(this->alturaInicTransicao, tempo_em_s(this->inicTransicao), this->status);

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

    // Só é preciso animar se o jogador estiver pulando ou caindo
    /* if(this->status != NORMAL && this->status != PLATAFORMA) {
        mult = zoom(altura);
        this->r = mult*this->r;
    } */
}
