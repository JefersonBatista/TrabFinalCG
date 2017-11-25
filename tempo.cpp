#include "tempo.h"

 unsigned long long iniciar() {
    struct timeval tv;

    gettimeofday(&tv, NULL);

    return
        (unsigned long long)(tv.tv_sec) * 1000 +
        (unsigned long long)(tv.tv_usec) / 1000;
}

unsigned long long tempo_em_ms(unsigned long long inicio) {
    struct timeval tv;

    gettimeofday(&tv, NULL);

    unsigned long long tempoAtual =
        (unsigned long long)(tv.tv_sec) * 1000 +
        (unsigned long long)(tv.tv_usec) / 1000;
    return tempoAtual - inicio;
}

float tempo_em_s(unsigned long long inicio) {
    return (float) (tempo_em_ms(inicio))/1000.0;
}
