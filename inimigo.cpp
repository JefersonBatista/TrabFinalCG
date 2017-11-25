#include "inimigo.h"

Inimigo::Inimigo(GLfloat centroX, GLfloat centroY, GLfloat raio, GLfloat vel, GLfloat velTiro, GLfloat freqTiro) {
    this->cx = centroX;
    this->cy = centroY;
    this->r = raio;
    this->ra = raio;
    this->front = 0.0;
    this->gun = 0.0;
    this->vel = vel;
    this->velTiro = velTiro;
    this->freqTiro = freqTiro;
    this->foot = RIGHTFOOT;
    this->status = NORMAL;
    this->lastFootChange = iniciar();
    this->lastShoot = iniciar();
    this->movStatus = FORWARD;
}

GLfloat Inimigo::getX() {
    return this->cx;
}

GLfloat Inimigo::getY() {
    return this->cy;
}

GLfloat Inimigo::getR() {
    return this->r;
}

int Inimigo::getMovStatus() {
    return this->movStatus;
}

void Inimigo::setMovStatus(int newMovStatus) {
    this->movStatus = newMovStatus;
}

void Inimigo::desenha() {
    GLfloat frontDeg = this->front/(2*M_PI)*360.0;

    glPushMatrix();
        glTranslatef(this->cx, this->cy, 0.0);
        glRotatef(frontDeg, 0.0, 0.0, 1.0);

        // Desenhando os pés
        switch(this->foot) {
            case RIGHTFOOT:
                desenhaRetangulo(this->ra/2.25, this->ra/1.2, 0.15, 0.0, 0.0);
                desenhaRetangulo(-this->ra/2.25, -this->ra/1.2, 0.15, 0.0, 0.0);
                break;
            case LEFTFOOT:
                desenhaRetangulo(-this->ra/2.25, this->ra/1.2, 0.15, 0.0, 0.0);
                desenhaRetangulo(this->ra/2.25, -this->ra/1.2, 0.15, 0.0, 0.0);
        }

        // Desenhando o braço
        glPushMatrix();
            GLfloat gunDeg = this->gun/(2*M_PI)*360.0;
            glTranslatef(this->ra/1.5, 0.0, 0.0);
            glRotatef(gunDeg, 0.0, 0.0, 1.0);
            desenhaRetangulo(this->ra/4.0, this->ra, 0.5, 0.0, 0.0);
        glPopMatrix();

        // Desenhando ombros
        desenhaElipse(this->ra, this->ra/3.0, 0.6, 0.0, 0.0);

        // Desenhando a cabeça
        desenhaCirc(this->ra/1.5, 1.0, 0.0, 0.0);
    glPopMatrix();
}

void Inimigo::move(Jogo *jogo, GLfloat delta, int num) {
    GLfloat incr = this->vel*delta;

    GLfloat pX = this->cx - incr*sin(this->front);
    GLfloat pY = this->cy + incr*cos(this->front);
    GLfloat r = this->r;

    GLfloat altura;
    int checagem = jogo->checarInimigos(pX, pY, r, num);

    // Decide se move o jogador
    switch(checagem) {
        case CHAO:
            this->cx = pX;
            this->cy = pY;
            this->toogleFoot();
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
                    altura = nivel(this->alturaInicTransicao, tempo_em_s(this->inicTransicao), this->status);
                    if (altura >= jogo->getAlturaObst()) {
                        this->cx = pX;
                        this->cy = pY;
                        this->toogleFoot();
                    } else {
                        return;
                    }
                case CAINDO:
                    altura = nivel(this->alturaInicTransicao, tempo_em_s(this->inicTransicao), this->status);
                    if (altura >= jogo->getAlturaObst()) {
                        this->cx = pX;
                        this->cy = pY;
                        this->toogleFoot();
                    } else {
                        return;
                    }
                case PLATAFORMA:
                    this->cx = pX;
                    this->cy = pY;
                    this->toogleFoot();
            }
        default:
            // Caso PROIBIDO, não faça o movimento
            return;
    }
}

void Inimigo::rotate(GLfloat delta) {
    GLfloat angleIncr = this->vel*delta/MOTION_TO_ROTATION;

    this->front += angleIncr;
    if(this->front > 2*M_PI) this->front -= 2*M_PI;
}

void Inimigo::toogleFoot() {
    if(tempo_em_ms(this->lastFootChange) < TEMPO_PASSO) {
        return;
    }
    switch (this->foot) {
        case RIGHTFOOT:
            this->foot = LEFTFOOT;
            break;
        case LEFTFOOT:
            this->foot = RIGHTFOOT;
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
    // Não atirar pulando ou em cima de plataforma
    if(this->status != NORMAL) {
        return;
    }

    GLfloat toArm = this->ra/1.5 + this->ra/8.0;
    GLfloat angle = this->front;

    GLfloat cx = this->cx + toArm*cos(angle);
    GLfloat cy = this->cy + toArm*sin(angle);
    GLfloat vel = this->velTiro;
    GLfloat dir = this->front + this->gun;

    jogo->adicionarTiro(Tiro(cx, cy, vel, dir, INIMIGO));
}

void Inimigo::pula(Jogo *jogo) {
    switch (this->status) {
        case NORMAL:
            alturaInicTransicao = 0.0;
            inicTransicao = iniciar();
            this->status = PULANDO;
            break;
        case PLATAFORMA:
            alturaInicTransicao = jogo->getAlturaObst();
            inicTransicao = iniciar();
            this->status = PULANDO;
    }
}

void Inimigo::anima(Jogo *jogo) {
    int checagem;
    GLfloat altura, mult;
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
            altura = nivel(this->alturaInicTransicao, tempo_em_s(this->inicTransicao), this->status);
            break;
        case CAINDO:
            checagem = jogo->checarLimites(this->cx, this->cy, this->r, INIMIGO);
            altura = nivel(this->alturaInicTransicao, tempo_em_s(this->inicTransicao), this->status);

            switch (checagem) {
                case CHAO:
                    // A queda acabou?
                    if(altura < 0.0) {
                        altura = 0.0;
                        this->status = NORMAL;
                    }
                    break;
                case OBSTACULO:
                    // A queda acabou?
                    if(altura < jogo->getAlturaObst()) {
                        altura = jogo->getAlturaObst();
                        this->status = PLATAFORMA;
                    }
            }
    }

    // Só é preciso animar se o inimigo estiver pulando ou caindo
    if(this->status != NORMAL && this->status != PLATAFORMA) {
        mult = zoom(altura);
        this->ra = mult*this->r;
    }

    GLfloat tempoPorTiro = 1.0/this->freqTiro;
    if(tempo_em_s(this->lastShoot) > tempoPorTiro) {
        this->atira(jogo);
        this->lastShoot = iniciar();
    }
}
