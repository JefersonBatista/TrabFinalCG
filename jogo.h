#ifndef JOGO_H
#define JOGO_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#include <math.h>
#include <vector>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tempo.h"
#include "util.h"
#include "obstaculo.h"
#include "tiro.h"
#include "arena.h"
#include "inimigo.h"
#include "jogador.h"

// Resolvendo referências cruzadas
class Inimigo;
class Jogador;

using namespace std;

class Jogo {
    Arena *arena;
    Jogador *jogador;
    vector<Inimigo> *inimigos;
    vector<Obstaculo> *obstaculos;
    vector<Tiro> *tiros;
    GLfloat alturaObst;
    int pontos;
    int status;

    public:
        Jogo(Arena *arena, Jogador *jogador, vector<Inimigo> *inimigos, vector<Obstaculo> *obstaculos, GLfloat alturaObst);
        void desenha(int braco);
        void mensagem();
        void minimapa();
        int checarLimites(GLfloat pX, GLfloat pY, GLfloat r, int personagem);
        int checarInimigos(GLfloat pX, GLfloat pY, GLfloat r, int num);
        int checarTiros(GLfloat pX, GLfloat pY, GLfloat pH, GLfloat r, int dono);
        void adicionarTiro(Tiro tiro);
        void anima();
        void moveInimigos(GLfloat transcorrido);
        int getStatus();
        void marca(int morto);
        void fim();
        GLfloat getAlturaObst();
};

#endif
