#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "jogo.h"

#define TOTAL_CARTAS 39

int main() {
    srand(time(NULL));

    PilhaCarta *listaCartas = lerArq("cartas.txt");
    Jogo *jogo = inicializa_estruturas(listaCartas);
    int modo;
    int aux = 1;
    while (aux == 1) {
        printf("Escolha o modo de jogo:\n");
        printf("1 - Manual\n");
        printf("2 - Automático\n");
        printf("3 - Encerrar\n");
        scanf("%d", &modo);
        switch (modo) {
            case 1:
                partidas_manual(jogo);
            case 2:
                //partidas_automatico(jogo);
            case 3:
                printf("Jogo Encerrado!");
                aux = 0;
                break;
        }
    }
}

Jogo *inicializa_estruturas(PilhaCarta *listCartas) {
    Jogador *jogadorA1 = (Jogador *) malloc(sizeof(Jogador));
    strcpy(jogadorA1->nome, "A1");
    jogadorA1->cartas = NULL;


    Jogador *jogadorB1 = (Jogador *) malloc(sizeof(Jogador));
    strcpy(jogadorB1->nome, "B1");
    jogadorB1->cartas = NULL;


    Jogador *jogadorA2 = (Jogador *) malloc(sizeof(Jogador));
    strcpy(jogadorA2->nome, "A2");
    jogadorA2->cartas = NULL;


    Jogador *jogadorB2 = (Jogador *) malloc(sizeof(Jogador));
    strcpy(jogadorB2->nome, "B2");
    jogadorB2->cartas = NULL;

    jogadorA1->ant = jogadorB2;
    jogadorA1->prox = jogadorB1;

    jogadorB1->ant = jogadorA1;
    jogadorB1->prox = jogadorA2;

    jogadorA2->ant = jogadorB1;
    jogadorA2->prox = jogadorB2;

    jogadorB2->ant = jogadorA2;
    jogadorB2->prox = jogadorA1;

    Mesa *mesa = (Mesa *) malloc(sizeof(Mesa));
    mesa->jogadores = jogadorA1;
    mesa->rodada = 0;

    Jogo *jogo = (Jogo *) malloc(sizeof(Jogo));
    jogo->mesa = mesa;
    jogo->pontosA = 0;
    jogo->pontosB = 0;
    jogo->turno = 0;
    jogo->vencedor = 'N';
    jogo->lcartas = listCartas;
    jogo->carta_virada = NULL;

    return jogo;
}

void partidas_manual(Jogo *jogo) {
    distribuir_cartas(jogo);

    while (verifica_vencedor(jogo) == 'N') {
        printf("-----Turno %d-----\n\n", jogo->turno + 1);
        for (jogo->mesa->rodada = 0; jogo->mesa->rodada < 3; jogo->mesa->rodada++) {
            printf("---Rodada %d---\n", jogo->mesa->rodada + 1);
            Jogador *jogador = jogo->jogador_que_distribuiu->prox;
            for (int j = 0; j < 4; j++) {
                printf("--Jogador %s--\n", jogador->nome);
                printf("Cartas:\n");

                PilhaCarta *cartas = jogador->cartas;
                jogo->carta_virada = escolherCarta(cartas);

                printf("Carta virada: %d de %s\n", jogo->carta_virada->valor, jogo->carta_virada->naipe);

                jogador = jogador->prox;
            }
        }
    }

    printf("Fim de jogo!\n");
    printf("O vencedor foi o time %c\n!", verifica_vencedor(jogo));
    printf("\nDeseja jogar novamente?\n");
}

char verifica_vencedor(Jogo *jogo) {
    if (jogo->pontosA >= 12) {
        return 'A';
    } else if (jogo->pontosB >= 12) {
        return 'B';
    } else {
        return 'N';
    }
}

Carta *escolherCarta(PilhaCarta *cartas) {
    PilhaCarta *aux = cartas->prox;
    int escolha = 0;
    while (aux != NULL) {
        escolha++;
        printf("%d - %d de %s\n", escolha, aux->valor->valor, aux->valor->naipe);
        aux = aux->prox;
    }

    printf("Escolha uma carta: ");
    scanf("%d", &escolha);
    Carta *carta = remover_carta(cartas, escolha);
    return carta;
}

void distribuir_cartas(Jogo *jogo) {
    if (jogo->turno == 0) {
        int sorteioDistribui = rand() % 4;
        Jogador *atual = jogo->mesa->jogadores;
        for (int k = 0; k < sorteioDistribui; k++) {
            atual = atual->prox;
        }
        jogo->jogador_que_distribuiu = atual;
    } else {
        jogo->jogador_que_distribuiu = jogo->jogador_que_distribuiu->prox;
    }

    PilhaCarta *baralho = embaralhar(jogo->lcartas);

    Jogador *jogadores = jogo->jogador_que_distribuiu;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
            adicionar_carta(jogadores->cartas, pop(baralho));
        }
        jogadores = jogadores->prox;
    }
}

// adicionar na pilha
void push(PilhaCarta *topo, Carta *valor) {
    PilhaCarta *atual = topo;

    while (atual->prox != NULL) {
        atual = atual->prox;
    }
    atual->prox = (PilhaCarta *) malloc(sizeof(PilhaCarta));
    atual->prox->valor = valor;
    atual->prox->prox = NULL;
}

// remover da pilha
Carta *pop(PilhaCarta *topo) {
    Carta *valor;
    if (topo->prox == NULL) {
        valor = topo->valor;
        free(topo);
        return valor;
    }

    PilhaCarta *atual = topo;
    while (atual->prox->prox != NULL) {
        atual = atual->prox;
    }

    valor = atual->prox->valor;
    free(atual->prox);
    atual->prox = NULL;
    return valor;
}

// adicionar na lista
void adicionar_carta(PilhaCarta *cartas, Carta *valor) {
    PilhaCarta *atual = cartas;
    if(atual == NULL) {
        atual = (PilhaCarta *) malloc(sizeof(PilhaCarta));
        atual->valor = valor;
        atual->prox = NULL;
        return;
    }

    while (atual->prox != NULL) {
        atual = atual->prox;
    }
    atual->prox = (PilhaCarta *) malloc(sizeof(PilhaCarta));
    atual->prox->valor = valor;
    atual->prox->prox = NULL;
}

// remover da lista
Carta *remover_carta(PilhaCarta *cartas, int posicao) {
    Carta *valor;
    PilhaCarta *atual = cartas;
    for (int i = 0; i < posicao; i++) {
        atual = atual->prox;
    }
    valor = atual->prox->valor;
    free(atual->prox);
    atual->prox = NULL;
    return valor;
}

PilhaCarta *embaralhar(PilhaCarta *topo) {
    PilhaCarta *baralho = (PilhaCarta *) malloc(sizeof(PilhaCarta));;
    baralho->valor = NULL;
    baralho->prox = NULL;

    PilhaCarta *cartas = topo;

    while (cartas->prox != NULL) {
        Carta *novaCarta = (Carta *) malloc(sizeof(Carta));
        novaCarta->valor = cartas->prox->valor->valor;
        strcpy(novaCarta->naipe, cartas->prox->valor->naipe);
        push(baralho, novaCarta);
        cartas = cartas->prox;
    }

    for (int indexCarta = 0; indexCarta < TOTAL_CARTAS - 1; indexCarta++) {
        int indexCartaTrocada = rand() % TOTAL_CARTAS;

        PilhaCarta *original = baralho->prox;
        for (int k = 0; k < indexCarta; k++) {
            original = original->prox;
        }

        PilhaCarta *trocada = baralho->prox;
        for (int k = 0; k < indexCartaTrocada; k++) {
            trocada = trocada->prox;
        }

        Carta *aux = original->valor;
        original->valor = trocada->valor;
        trocada->valor = aux;
    }
    return baralho;
}

PilhaCarta *lerArq(char *nome) {
    PilhaCarta *topo = NULL;
    topo = (PilhaCarta *) malloc(sizeof(PilhaCarta));
    if (topo == NULL) {
        exit(1);
    }

    FILE *file = fopen("cartas.txt", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    char linha[20];
    while (fgets(linha, 20, file) != NULL) {
        Carta *novaCarta = (Carta *) malloc(sizeof(Carta));
        sscanf(linha, " %c %[^\n]", &novaCarta->valor, novaCarta->naipe);
        push(topo, novaCarta);
    }

    fclose(file);
    return topo;
}