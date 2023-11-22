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
    mesa->pontosA = 0;
    mesa->pontosB = 0;

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

    while (verifica_vencedor(jogo) == 'N') {
        jogo->turno++;
        printf("-----Turno %d-----\n", jogo->turno);
        PilhaCarta * baralho = distribuir_cartas(jogo);
        jogo->carta_virada = pop(baralho);

        for (jogo->mesa->rodada = 0; jogo->mesa->rodada < 3; jogo->mesa->rodada++) {
            printf("\n----Rodada %d----\n", jogo->mesa->rodada + 1);
            printf("Carta virada: %c de %s\n", jogo->carta_virada->valor, jogo->carta_virada->naipe);

            int ponto = 1;

            Jogador *jogador = jogo->jogador_que_distribuiu->prox;
            Carta * maiorCarta = NULL;
            char timeMaiorCarta = 'N';
            int empate = 0;
            for (int j = 0; j < 4; j++) {
                printf("\n--Jogador %s--\n", jogador->nome);
                printf("Cartas:\n");

                PilhaCarta *cartas = jogador->cartas;
                Carta * carta = escolherCarta(cartas);

                if(maiorCarta != NULL && strcmp(carta->naipe, maiorCarta->naipe) == 0 && carta->valor == maiorCarta->valor) {
                    empate = 1;
                } else {
                    char maior = verifica_carta(carta, maiorCarta, jogo->carta_virada, jogo->lcartas);
                    if(maior == '1') {
                        timeMaiorCarta = jogador->nome[0];
                        maiorCarta = carta;
                    }
                    empate = 0;
                }

                printf("Carta jogada: %c de %s\n", carta->valor, carta->naipe);
                jogador = jogador->prox;
            }

            if(timeMaiorCarta == 'A') {
                jogo->mesa->pontosA++;
            } else if(timeMaiorCarta == 'B') {
                jogo->mesa->pontosB++;
            }

            if(empate == 1) {
                jogo->mesa->pontosA++;
                jogo->mesa->pontosB++;
            }

            if (jogo->mesa->pontosA >= 2) {
                jogo->pontosA = jogo->pontosA + ponto;
                printf("Fim da rodada! O time A ganhou %d pontos!\n", ponto);
                break;
            } else if(jogo->mesa->pontosB >= 2) {
                jogo->pontosB = jogo->pontosB + ponto;
                printf("Fim da rodada! O time B ganhou %d pontos!\n", ponto);
                break;
            }
        }
    }

    printf("Fim de jogo!\n");
    printf("O vencedor foi o time %c\n!", verifica_vencedor(jogo));
    printf("\nDeseja jogar novamente?\n");
}

char verifica_carta(Carta *carta1, Carta * carta2, Carta * vira, PilhaCarta *ordemCartas) {
    if(carta1 == NULL) {
        return '2';
    } else if(carta2 == NULL) {
        return '1';
    }

    PilhaCarta *aux = ordemCartas;
    Carta *manilha = NULL;

    int ordemCarta = 10;
    int ordemCarta1;
    int ordemCarta2;

    char valor = '0';
    while(aux->prox != NULL) {
        if(aux->prox->valor->valor != valor) {
            valor = aux->prox->valor->valor;
            if (aux->prox->valor->valor == vira->valor) {
                manilha = aux->prox->prox->prox->prox->prox->valor;
                break;
            }
        }
        aux = aux->prox;
    }

    if (carta1->valor == manilha->valor && carta2->valor != manilha->valor) {
        return '1';
    } else if  (carta2->valor == manilha->valor && carta1->valor != manilha->valor){
        return '2';
    } else if(carta1->valor == manilha->valor && carta2->valor == manilha->valor) {
        ordemCarta1 = verifica_naipe(carta1);
        ordemCarta2 = verifica_naipe(carta2);
    } else {
        valor = '0';
        aux = ordemCartas;
        while(aux->prox != NULL) {
            if(aux->prox->valor->valor != valor) {
                valor = aux->prox->valor->valor;

                if(aux->prox->valor->valor == carta1->valor) {
                    ordemCarta1 = ordemCarta;
                }

                if(aux->prox->valor->valor == carta2->valor) {
                    ordemCarta2 = ordemCarta;
                }
                ordemCarta--;
            }
            aux = aux->prox;
        }
    }

    if(ordemCarta1 > ordemCarta2) {
        return '2';
    } else if(ordemCarta1 < ordemCarta2) {
        return '1';
    } else {
        return '0';
    }
}

int verifica_naipe(Carta * carta) {
    int ordemCarta;
    if(strcmp(carta->naipe, "Paus") == 0) {
        ordemCarta = 1;
    } else if(strcmp(carta->naipe, "Copas") == 0) {
        ordemCarta = 2;
    } else if(strcmp(carta->naipe, "Espadas") == 0) {
        ordemCarta = 3;
    } else if(strcmp(carta->naipe, "Ouros") == 0) {
        ordemCarta = 4;
    }
    return ordemCarta;
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
    PilhaCarta *aux = cartas;
    int escolha = 0;
    while (aux != NULL) {
        escolha++;
        printf("%d - %c de %s\n", escolha, aux->valor->valor, aux->valor->naipe);
        aux = aux->prox;
    }

    printf("Escolha uma carta: ");
    scanf("%d", &escolha);
    Carta *carta = remover_carta(cartas, escolha);
    return carta;
}

PilhaCarta * distribuir_cartas(Jogo *jogo) {
    if (jogo->turno == 1) {
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
            jogadores->cartas = adicionar_carta(jogadores->cartas, pop(baralho));
        }
        jogadores = jogadores->prox;
    }
    return baralho;
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
PilhaCarta * adicionar_carta(PilhaCarta *cartas, Carta *valor) {
    PilhaCarta *atual = cartas;
    if(atual == NULL) {
        atual = (PilhaCarta *) malloc(sizeof(PilhaCarta));
        atual->valor = valor;
        atual->prox = NULL;
        return atual;
    }

    while (atual->prox != NULL) {
        atual = atual->prox;
    }
    atual->prox = (PilhaCarta *) malloc(sizeof(PilhaCarta));
    atual->prox->valor = valor;
    atual->prox->prox = NULL;
    return cartas;
}

Carta * remover_carta(PilhaCarta *cartas, int posicao) {
    PilhaCarta * atual = cartas;
    PilhaCarta * aux = NULL;
    Carta * valor = NULL;
    switch (posicao) {
        case 1:
            valor = cartas->valor;
            PilhaCarta *temp = cartas;
            cartas = cartas->prox;
            free(temp);
            break;
        case 2:
            valor = atual->prox->valor;
            aux = atual->prox->prox;
            free(atual->prox);
            atual->prox = aux;
            break;
        case 3:
            valor = atual->prox->prox->valor;
            free(atual->prox->prox);
            atual->prox->prox = NULL;
            break;
    }
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