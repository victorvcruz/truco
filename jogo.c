#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "jogo.h"

#define TOTAL_CARTAS 39

int main() {
    PilhaCarta *topo = NULL;
    topo = (PilhaCarta *) malloc(sizeof(PilhaCarta));
    if (topo == NULL) {
        return 1;
    }

    iniciarBaralho(topo);

    PilhaCarta *aux = topo->prox;
    while (aux->prox != NULL) {
        printf("%d de %s\n", aux->valor->numero, aux->valor->naipe);
        aux = aux->prox;
    }

    printf("\nEMBARALHADAS:\n");
    embaralhar(topo);

    aux = topo->prox;
    while (aux->prox != NULL) {
        printf("%d de %s\n", aux->valor->numero, aux->valor->naipe);
        aux = aux->prox;
    }
    printf("%d de %s\n", aux->valor->numero, aux->valor->naipe);
}

void push(PilhaCarta * topo, Carta *valor) {
    PilhaCarta * atual = topo;

    while(atual->prox != NULL) {
        atual = atual->prox;
    }
    atual->prox = (PilhaCarta*) malloc(sizeof(PilhaCarta));
    atual->prox->valor = valor;
    atual->prox->prox = NULL;
}

Carta * pop(PilhaCarta * topo) {
    struct Carta * valor;
    if (topo->prox == NULL) {
        valor = topo->valor;
        free(topo);
        return valor;
    }

    PilhaCarta * atual = topo;
    while (atual->prox->prox != NULL) {
        atual = atual->prox;
    }

    valor = atual->prox->valor;
    free(atual->prox);
    atual->prox = NULL;
    return valor;
}

void embaralhar(PilhaCarta *topo) {
    srand(time(NULL));
    for (int indexCarta = 0; indexCarta < TOTAL_CARTAS - 1; indexCarta++) {
        int indexCartaTrocada = rand() % TOTAL_CARTAS;

        PilhaCarta *original = topo->prox;
        for (int k = 0; k < indexCarta; k++) {
            original = original->prox;
        }

        PilhaCarta *trocada = topo->prox;
        for (int k = 0; k < indexCartaTrocada; k++) {
            trocada = trocada->prox;
        }

        Carta *aux = original->valor;
        original->valor = trocada->valor;
        trocada->valor = aux;
    }
}

void iniciarBaralho(PilhaCarta *topo) {
    Carta * carta;
    //4
    carta = (Carta *) malloc(sizeof(Carta));
    carta->numero = 4;
    strcpy(carta->naipe, "Ouro");
    push(topo, carta);

    carta = (Carta *)malloc(sizeof(Carta));
    carta->numero = 4;
    strcpy(carta->naipe, "Espada");
    push(topo, carta);

    carta = (Carta *)malloc(sizeof(Carta));
    carta->numero = 4;
    strcpy(carta->naipe, "Copas");
    push(topo, carta);

    carta = (Carta *)malloc(sizeof(Carta));
    carta->numero = 4;
    strcpy(carta->naipe, "Paus");
    push(topo, carta);

    //5
    carta = (Carta *)malloc(sizeof(Carta));
    carta->numero = 5;
    strcpy(carta->naipe, "Ouro");
    push(topo, carta);

    carta = (Carta *)malloc(sizeof(Carta));
    carta->numero = 5;
    strcpy(carta->naipe, "Espada");
    push(topo, carta);

    carta = (Carta *)malloc(sizeof(Carta));
    carta->numero = 5;
    strcpy(carta->naipe, "Copas");
    push(topo, carta);

    carta = (Carta *)malloc(sizeof(Carta));
    carta->numero = 5;
    strcpy(carta->naipe, "Paus");
    push(topo, carta);

    //6
    carta = (Carta *)malloc(sizeof(Carta));
    carta->numero = 6;
    strcpy(carta->naipe, "Ouro");
    push(topo, carta);

    carta = (Carta *)malloc(sizeof(Carta));
    carta->numero = 6;
    strcpy(carta->naipe, "Espada");
    push(topo, carta);

    carta = (Carta *)malloc(sizeof(Carta));
    carta->numero = 6;
    strcpy(carta->naipe, "Copas");
    push(topo, carta);

    carta = (Carta *)malloc(sizeof(Carta));
    carta->numero = 6;
    strcpy(carta->naipe, "Paus");
    push(topo, carta);

    //7
    carta = (Carta *)malloc(sizeof(Carta));
    carta->numero = 7;
    strcpy(carta->naipe, "Ouro");
    push(topo, carta);

    carta = (Carta *)malloc(sizeof(Carta));
    carta->numero = 7;
    strcpy(carta->naipe, "Espada");
    push(topo, carta);

    carta = (Carta *)malloc(sizeof(Carta));
    carta->numero = 7;
    strcpy(carta->naipe, "Copas");
    push(topo, carta);

    carta = (Carta *)malloc(sizeof(Carta));
    carta->numero = 7;
    strcpy(carta->naipe, "Paus");
    push(topo, carta);


    //Q
    carta = (Carta *)malloc(sizeof(Carta));
    carta->numero = 11;
    strcpy(carta->naipe, "Ouro");
    push(topo, carta);

    carta = (Carta *)malloc(sizeof(Carta));
    carta->numero = 11;
    strcpy(carta->naipe, "Espada");
    push(topo, carta);

    carta = (Carta *)malloc(sizeof(Carta));
    carta->numero = 11;
    strcpy(carta->naipe, "Copas");
    push(topo, carta);

    carta = (Carta *)malloc(sizeof(Carta));
    carta->numero = 11;
    strcpy(carta->naipe, "Paus");
    push(topo, carta);

    //J
    carta = (Carta *)malloc(sizeof(Carta));
    carta->numero = 12;
    strcpy(carta->naipe, "Ouro");
    push(topo, carta);

    carta = (Carta *)malloc(sizeof(Carta));
    carta->numero = 12;
    strcpy(carta->naipe, "Espada");
    push(topo, carta);

    carta = (Carta *)malloc(sizeof(Carta));
    carta->numero = 12;
    strcpy(carta->naipe, "Copas");
    push(topo, carta);

    carta = (Carta *)malloc(sizeof(Carta));
    carta->numero = 12;
    strcpy(carta->naipe, "Paus");
    push(topo, carta);

    //K
    carta = (Carta *)malloc(sizeof(Carta));
    carta->numero = 13;
    strcpy(carta->naipe, "Ouro");
    push(topo, carta);

    carta = (Carta *)malloc(sizeof(Carta));
    carta->numero = 13;
    strcpy(carta->naipe, "Espada");
    push(topo, carta);

    carta = (Carta *)malloc(sizeof(Carta));
    carta->numero = 13;
    strcpy(carta->naipe, "Copas");
    push(topo, carta);

    carta = (Carta *)malloc(sizeof(Carta));
    carta->numero = 13;
    strcpy(carta->naipe, "Paus");
    push(topo, carta);

    //A
    carta = (Carta *)malloc(sizeof(Carta));
    carta->numero = 1;
    strcpy(carta->naipe, "Ouro");
    push(topo, carta);

    carta = (Carta *)malloc(sizeof(Carta));
    carta->numero = 1;
    strcpy(carta->naipe, "Espada");
    push(topo, carta);

    carta = (Carta *)malloc(sizeof(Carta));
    carta->numero = 1;
    strcpy(carta->naipe, "Copas");
    push(topo, carta);

    carta = (Carta *)malloc(sizeof(Carta));
    carta->numero = 1;
    strcpy(carta->naipe, "Paus");
    push(topo, carta);

    //2
    carta = (Carta *)malloc(sizeof(Carta));
    carta->numero = 2;
    strcpy(carta->naipe, "Ouro");
    push(topo, carta);

    carta = (Carta *)malloc(sizeof(Carta));
    carta->numero = 2;
    strcpy(carta->naipe, "Espada");
    push(topo, carta);

    carta = (Carta *)malloc(sizeof(Carta));
    carta->numero = 2;
    strcpy(carta->naipe, "Copas");
    push(topo, carta);

    carta = (Carta *)malloc(sizeof(Carta));
    carta->numero = 2;
    strcpy(carta->naipe, "Paus");
    push(topo, carta);

    //3
    carta = (Carta *)malloc(sizeof(Carta));
    carta->numero = 3;
    strcpy(carta->naipe, "Ouro");
    push(topo, carta);

    carta = (Carta *)malloc(sizeof(Carta));
    carta->numero = 3;
    strcpy(carta->naipe, "Espada");
    push(topo, carta);

    carta = (Carta *)malloc(sizeof(Carta));
    carta->numero = 3;
    strcpy(carta->naipe, "Copas");
    push(topo, carta);

    carta = (Carta *)malloc(sizeof(Carta));
    carta->numero = 3;
    strcpy(carta->naipe, "Paus");
    push(topo, carta);
}