#include "jogo.h"

Jogo::Jogo(Arena *arena, Jogador *jogador, vector<Inimigo> *inimigos, vector<Obstaculo> *obstaculos, GLfloat alturaObst) {
    this->arena = arena;
    this->jogador = jogador;
    this->inimigos = inimigos;
    this->obstaculos = obstaculos;
    this->alturaObst = this->jogador->getAlturaPulo()*alturaObst;
    this->tiros = new vector<Tiro>;
    this->pontos = 0;
    this->status = JOGANDO;
}

void Jogo::desenha() {
    this->arena->desenha3d();

    for(int i = 0; i < obstaculos->size(); i++) {
        (*(this->obstaculos))[i].desenha3d(this->alturaObst);
    }

    for(int i = 0; i < tiros->size(); i++) {
        (*(this->tiros))[i].desenha3d();
    }

    if(this->status != PERDEU)
        this->jogador->desenha3d();

    for(int i = 0; i < inimigos->size(); i++) {
        (*(this->inimigos))[i].desenha3d();
    }
}

void Jogo::minimapa() {
    this->arena->desenha2d();

    for(int i = 0; i < obstaculos->size(); i++) {
        (*(this->obstaculos))[i].desenha2d();
    }

    if(this->status != PERDEU)
        this->jogador->desenha2d();

    for(int i = 0; i < inimigos->size(); i++) {
        (*(this->inimigos))[i].desenha2d();
    }
}

int Jogo::checarLimites(GLfloat pX, GLfloat pY, GLfloat r, int personagem) {
    // Checando limites externos da arena
    if(distancia(this->arena->getXBlue(), this->arena->getYBlue(), pX, pY) > this->arena->getRBlue() - r)
        return PROIBIDO;

    // Checando limites internos da arena
    if(distancia(this->arena->getXWhite(), this->arena->getYWhite(), pX, pY) < this->arena->getRWhite() + r)
        return PROIBIDO;

    // Checando colisão com jogador
    if(personagem != JOGADOR) {
        if(distancia(this->jogador->getX(), this->jogador->getY(), pX, pY) < this->jogador->getR() + r)
            return PROIBIDO;
    }

    // Checando colisão com inimigos
    if(personagem != INIMIGO) {
        for(int i = 0; i < inimigos->size(); i++) {
            if(distancia((*(this->inimigos))[i].getX(), (*(this->inimigos))[i].getY(), pX, pY) < (*(this->inimigos))[i].getR() + r)
                return PROIBIDO;
        }
    }

    // Checando obstáculos
    for(int i = 0; i < obstaculos->size(); i++) {
        if(distancia((*(this->obstaculos))[i].getX(), (*(this->obstaculos))[i].getY(), pX, pY) < (*(this->obstaculos))[i].getR() + r)
            return OBSTACULO;
    }

    return CHAO;
}

int Jogo::checarInimigos(GLfloat pX, GLfloat pY, GLfloat r, int num) {
    // Checando colisão com outros inimigos
    for(int i = 0; i < inimigos->size(); i++) {
        if(i != num) {
            if(distancia((*(this->inimigos))[i].getX(), (*(this->inimigos))[i].getY(), pX, pY) < (*(this->inimigos))[i].getR() + r)
                return PROIBIDO;
        }
    }
    return checarLimites(pX, pY, r, INIMIGO);
}

int Jogo::checarTiros(GLfloat pX, GLfloat pY, GLfloat pH, GLfloat r, int dono) {
    switch (dono) {
        case JOGADOR:
            for(int i = 0; i < inimigos->size(); i++) {
                if(distancia((*(this->inimigos))[i].getX(), (*(this->inimigos))[i].getY(), pX, pY) < (*(this->inimigos))[i].getR() + r) {
                    if(pH > (*(this->inimigos))[i].getH() && pH < (*(this->inimigos))[i].getH() + (*(this->inimigos))[i].getAltura()) {
                        this->marca(i);
                        return PROIBIDO;
                    }
                }
            }
            break;
        case INIMIGO:
            if(distancia(this->jogador->getX(), this->jogador->getY(), pX, pY) < this->jogador->getR() + r) {
                if(pH > this->jogador->getH() && pH < this->jogador->getH() + this->jogador->getAltura()) {
                    this->fim();
                    return PROIBIDO;
                }
            }
    }

    // Checando limites externos da arena
    if(distancia(this->arena->getXBlue(), this->arena->getYBlue(), pX, pY) > this->arena->getRBlue() - r)
        return PROIBIDO;

    // Checando limites internos da arena
    if(distancia(this->arena->getXWhite(), this->arena->getYWhite(), pX, pY) < this->arena->getRWhite() + r)
        return PROIBIDO;

    // Checando obstáculos
    for(int i = 0; i < obstaculos->size(); i++) {
        if(distancia((*(this->obstaculos))[i].getX(), (*(this->obstaculos))[i].getY(), pX, pY) < (*(this->obstaculos))[i].getR() + r) {
            if(pH < this->alturaObst) {
                return OBSTACULO;
            }
        }
    }

    return CHAO;
}

void Jogo::adicionarTiro(Tiro tiro) {
    this->tiros->push_back(tiro);
}

void Jogo::anima() {
    this->jogador->anima(this);

    for(int i = 0; i < inimigos->size(); i++) {
        (*(this->inimigos))[i].anima(this);
    }

    // Animando os tiros
    GLfloat pX, pY, pH;
    int numTiros = this->tiros->size();
    for(int i = 0, j = 0; i < numTiros; i++) {
        pX = (*tiros)[j].nextX();
        pY = (*tiros)[j].nextY();
        pH = (*tiros)[j].nextH();

        int checagem = checarTiros(pX, pY, pH, RAIO_TIRO, (*tiros)[j].getDono());

        if(checagem == CHAO) {
            (*tiros)[j].setX(pX);
            (*tiros)[j].setY(pY);
            (*tiros)[j].setH(pH);
            j++;
        } else {
            this->tiros->erase(this->tiros->begin() + j);
        }
    }
}

void Jogo::moveInimigos(GLfloat transcorrido) {
    Inimigo *atual;
    Obstaculo *ob;
    GLfloat x, y, r, xar, yar, rar, xob, yob, rob, dist0, dist, distOb0, distOb;
    int movStatus, obProx;

    for(int i = 0; i < inimigos->size(); i++) {
        atual = &((*inimigos)[i]);
        x = atual->getX();
        y = atual->getY();
        r = atual->getR();
        movStatus = atual->getMovStatus();

        // Checagem preliminar de obstáculos
        obProx = 0;
        for(int j = 0; j < obstaculos->size(); j++) {
            ob = &((*obstaculos)[j]);
            xob = ob->getX();
            yob = ob->getY();
            rob = ob->getR();
            distOb0 = (distancia(x, y, xob, yob) - rob) - r;

            if(distOb0 < r) {
                obProx = 1;
                break;
            }
        }
        // Fim da checagem preliminar de obstáculos

        switch(movStatus) {
            case CURVE_IN:
                xar = this->arena->getXWhite();
                yar = this->arena->getYWhite();
                rar = this->arena->getRWhite();
                dist0 = (distancia(x, y, xar, yar) - rar) - r;

                if(i % 2 == 0) {
                    atual->rotate(+transcorrido);
                } else {
                    atual->rotate(-transcorrido);
                }
                break;
            case FLEE_IN:
                xar = this->arena->getXWhite();
                yar = this->arena->getYWhite();
                rar = this->arena->getRWhite();
                dist0 = (distancia(x, y, xar, yar) - rar) - r;
                break;

            case CURVE_OUT:
                xar = this->arena->getXBlue();
                yar = this->arena->getYBlue();
                rar = this->arena->getRBlue();
                dist0 = (rar - distancia(x, y, xar, yar)) - r;

                if(i % 2 == 0) {
                    atual->rotate(-transcorrido);
                } else {
                    atual->rotate(+transcorrido);
                }
                break;
            case FLEE_OUT:
                xar = this->arena->getXBlue();
                yar = this->arena->getYBlue();
                rar = this->arena->getRBlue();
                dist0 = (rar - distancia(x, y, xar, yar)) - r;
        }

        // Todos os inimigos são movidos
        atual->move(this, transcorrido, i);

        // Atualizando posição após o movimento
        x = atual->getX();
        y = atual->getY();

        // printf("movStatus= %d\n", movStatus);
        if(movStatus == FORWARD) {
            // printf("Voltando ao normal.\n");
            xar = this->arena->getXWhite();
            yar = this->arena->getYWhite();
            rar = this->arena->getRWhite();
            dist = (distancia(x, y, xar, yar) - rar) - r;

            // printf("dist = %f, r = %f\n", dist, r);
            if (dist < r) {
                atual->setMovStatus(CURVE_IN);
            }

            xar = this->arena->getXBlue();
            yar = this->arena->getYBlue();
            rar = this->arena->getRBlue();
            dist = (rar - distancia(x, y, xar, yar)) - r;

            if (dist < r) {
                atual->setMovStatus(CURVE_OUT);
            }
        } else {
            // case in
            if(movStatus == CURVE_IN || movStatus == FLEE_IN) {
                // Atualizando a distância para o limite interno da arena
                dist = (distancia(x, y, xar, yar) - rar) - r;
            }

            // case out
            else {
                // Atualizando a distância para o limite externo da arena
                dist = (rar - distancia(x, y, xar, yar)) - r;
            }

            if(dist > dist0) {
                switch (movStatus) {
                    case CURVE_IN:
                        atual->setMovStatus(FLEE_IN);
                        break;
                    case FLEE_IN:
                        if (dist > r) {
                            atual->setMovStatus(FORWARD);
                        }
                        break;
                    case CURVE_OUT:
                        atual->setMovStatus(FLEE_OUT);
                        break;
                    case FLEE_OUT:
                        if (dist > r) {
                            atual->setMovStatus(FORWARD);
                        }
                }
            } else {
                switch (movStatus) {
                    case FLEE_IN:
                        atual->setMovStatus(CURVE_IN);
                        break;
                    case FLEE_OUT:
                        atual->setMovStatus(CURVE_OUT);
                }
            }
        }

        // Checagem posterior de obstáculos
        if(!obProx) {
            continue;
        }

        // Atualizando distância até o obstáculo próximo
        distOb = (distancia(x, y, xob, yob) - rob) - r;

        // Obstáculo à vista
        if(distOb < distOb0) {
            atual->pula(this);
        }
        // Fim da checagem posterior de obstáculos
    }
}

void Jogo::marca(int morto) {
    this->pontos += 1;
    this->inimigos->erase(this->inimigos->begin() + morto);

    // Verificando se o jogador venceu
    if(this->inimigos->size() == 0 && this->status != PERDEU) {
        this->status = VENCEU;
    }
}

void Jogo::fim() {
    if(this->status != VENCEU)
        this->status = PERDEU;
}

GLfloat Jogo::getAlturaObst() {
    return this->alturaObst;
}
