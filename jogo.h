#ifndef TRUCO_JOGO_H
#define TRUCO_JOGO_H

typedef struct carta {
    int numero;
    char naipe[15];
} Carta;

typedef struct PilhaCarta {
    Carta *valor;
    struct PilhaCarta *prox;
} PilhaCarta;

void push(PilhaCarta * topo, Carta *valor);

Carta * pop(PilhaCarta * topo);

void embaralhar(PilhaCarta *topo);

void iniciarBaralho(PilhaCarta *topo);

#endif //TRUCO_JOGO_H
