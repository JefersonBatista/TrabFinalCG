#include "jogo.h"

Jogo::Jogo(Arena *arena, Jogador *jogador, vector<Inimigo> *inimigos, vector<Obstaculo> *obstaculos, GLfloat alturaObst) {
    this->arena = arena;
    this->jogador = jogador;
    this->inimigos = inimigos;
    this->obstaculos = obstaculos;
    this->alturaObst = alturaObst;
    this->tiros = new vector<Tiro>;
    this->pontos = 0;
    this->status = JOGANDO;
}

void Jogo::desenha(GLfloat minX, GLfloat maxX, GLfloat minY, GLfloat maxY) {
    this->arena->desenha();

    for(int i = 0; i < obstaculos->size(); i++) {
        (*(this->obstaculos))[i].desenha();
    }

    for(int i = 0; i < tiros->size(); i++) {
        (*(this->tiros))[i].desenha();
    }

    if(this->status != PERDEU)
        this->jogador->desenha();

    for(int i = 0; i < inimigos->size(); i++) {
        (*(this->inimigos))[i].desenha();
    }

    char text[20], vitoria[20], derrota[20];
    sprintf(text, "Pontos: %d", this->pontos);

    glPushMatrix();
        glTranslatef(minX + 24.0, maxY - 24.0, 0.0);
        glColor3f(0.75, 0.0, 0.0);
        glRasterPos2f(maxX - 400.0, 0.0);

        for(int i = 0; i < strlen(text); i++) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
        }

        switch (this->status) {
            case VENCEU:
                sprintf(vitoria, "Voce venceu!");

                glRasterPos2f(maxX - 400.0, -24.0);

                for(int i = 0; i < strlen(vitoria); i++) {
                    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, vitoria[i]);
                }
                break;
            case PERDEU:
                sprintf(derrota, "Voce perdeu!");
                glRasterPos2f(maxX - 400.0, -24.0);

                for(int i = 0; i < strlen(derrota); i++) {
                    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, derrota[i]);
                }
                break;
        }

    glPopMatrix();
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

int Jogo::checarTiros(GLfloat pX, GLfloat pY, GLfloat r, int dono) {
    switch (dono) {
        case JOGADOR:
            for(int i = 0; i < inimigos->size(); i++) {
                if(distancia((*(this->inimigos))[i].getX(), (*(this->inimigos))[i].getY(), pX, pY) < (*(this->inimigos))[i].getR() + r) {
                    this->marca(i);
                    return PROIBIDO;
                }
            }
            break;
        case INIMIGO:
            if(distancia(this->jogador->getX(), this->jogador->getY(), pX, pY) < this->jogador->getR() + r) {
                this->fim();
                return PROIBIDO;
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
        if(distancia((*(this->obstaculos))[i].getX(), (*(this->obstaculos))[i].getY(), pX, pY) < (*(this->obstaculos))[i].getR() + r)
            return OBSTACULO;
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
    GLfloat pX, pY;
    int numTiros = this->tiros->size();
    for(int i = 0, j = 0; i < numTiros; i++) {
        pX = (*tiros)[j].nextX();
        pY = (*tiros)[j].nextY();

        int checagem = checarTiros(pX, pY, RAIO_TIRO, (*tiros)[j].getDono());

        if(checagem == CHAO) {
            (*tiros)[j].setX(pX);
            (*tiros)[j].setY(pY);
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

        if(movStatus == FORWARD) {
            xar = this->arena->getXWhite();
            yar = this->arena->getYWhite();
            rar = this->arena->getRWhite();
            dist = (distancia(x, y, xar, yar) - rar) - r;

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
