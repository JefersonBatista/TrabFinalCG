#ifndef TEMPO_H
#define TEMPO_H

#include <stdlib.h>
#include <sys/time.h>

// Inicia uma nova contagem de tempo
unsigned long long iniciar();

// Retorna o tempo transcorrido de uma contagem, em ms
unsigned long long tempo_em_ms(unsigned long long inicio);

// Retorna o tempo transcorrido de uma contagem, em s
float tempo_em_s(unsigned long long inicio);

#endif
